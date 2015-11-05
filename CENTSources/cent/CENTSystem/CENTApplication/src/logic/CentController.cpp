#include "CentController.h"

#include <QSettings>
#include <QDateTime>

// General
#include "CentActionHandler.h"
#include "CentGameManager.h"
#include "CentMainWindow.h"
#include "CentMainWindowController.h"
#include "CentStateManager.h"
#include "Connect.h"
#include "LogicUiFusion.h"
#include "ErrorHandler.h"
#include "MainWindowCommon.h"
#include "SessionNotesLauncher.h"

// OpenVibe
#include "VRPNClientThread.h"
#include "VRPNServerThread.h"
#include "OVAppsRunner.h"

// Page controllers
#include "BaselinePageController.h"
#include "BaselineInfoPageController.h"
#include "ChooseScenarioGroupPageController.h"
#include "EvaluationPageController.h"
#include "PatientQuestionaryPageController.h"
#include "GamePageController.h"
#include "SetupCapPageController.h"
#include "SummaryPageController.h"
#include "GameSelectPageController.h"
#include "TutorialPageController.h"
#include "WelcomePageController.h"
#include "SummaryController.h"
#include "SessionFeedbackPageController.h"
#include "FinishPageController.h"

// Data management
#include "CentUserData.h"
#include "PatientDataCenter.h"

// Debug stuff
#include "DebugOutputManager.h"

CentController::CentController(QSettings& settings, CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: QObject(parent)
	, m_researcherWindow(researcherWindow)
	, m_patientWindow(patientWindow)
	, m_settings(settings)
	, m_centWindowController(new CentMainWindowController(researcherWindow, patientWindow, this))
	, m_vrpnClientThread(0)
	, m_vrpnServerThread(0)
	, m_gameManager(new CentGameManager(this))
	, m_logicUiFusion(0)
	, m_stateManager(0)
	, m_actionHandler(0)
	, m_welcomePageController(new WelcomePageController(researcherWindow, patientWindow, this))
	, m_patientQuestionaryPageController(0)
	, m_setupCapPageController(new SetupCapPageController(researcherWindow, patientWindow, this))
	, m_baselinePageController(new BaselinePageController(researcherWindow, patientWindow, this))
	, m_baselineInfoPageController(new BaselineInfoPageController(researcherWindow, patientWindow, this))
	, m_gameSelectPageController(new GameSelectPageController(researcherWindow, patientWindow, this))
	, m_tutorialPageController(new TutorialPageController(researcherWindow, patientWindow, this))
	, m_gamePageController(0)
	, m_summaryController(new SummaryController(this))
	, m_chooseScenarioGroupPageController(new ChooseScenarioGroupPageController(researcherWindow, patientWindow, this))
	, m_sessionFeedbackPageController(new SessionFeedbackPageController(researcherWindow, patientWindow, this))
	, m_evaluationPageController(new EvaluationPageController(researcherWindow, patientWindow, this))
	, m_finishPageController(new FinishPageController(researcherWindow, patientWindow, this))
	, m_patientDataCenter(new PatientDataCenter(this))
	, m_errorHandler(new ErrorHandler(researcherWindow, this))
	, m_sessionNotesLauncher(new SessionNotesLauncher(this))
{
	m_logicUiFusion            = new LogicUiFusion<CentState, CentUiState>(*m_centWindowController);
	m_stateManager             = new CentStateManager(*m_logicUiFusion, this);
	m_gamePageController       = new GamePageController(researcherWindow, patientWindow, *m_summaryController, this);
	m_summaryPageController    = new SummaryPageController(researcherWindow, patientWindow, *m_summaryController, this);
	m_patientQuestionaryPageController = new PatientQuestionaryPageController(researcherWindow, patientWindow, *m_patientDataCenter, this);

	m_actionHandler = new CentActionHandler(*m_stateManager, this);
	m_vrpnServerThread = new VRPNServerThread(this);
	m_ovAppsRunner = new OVAppsRunner(settings, *m_vrpnServerThread, *m_patientDataCenter, this);

	//setup menu for launching session notes
	QAction* sessionNotesAction = m_centWindowController->registerMenuItem(QString(CENT::Menu::TOOLS_MENU_NAME), tr(CENT::Menu::SESSION_NOTES_ACTION_NAME));
	sessionNotesAction->setEnabled(false);
	m_sessionNotesLauncher->setSessionNotesAction(sessionNotesAction);

	m_actionHandler->setOVAppsRunner(m_ovAppsRunner);
	m_actionHandler->setGameManager(m_gameManager);
	m_actionHandler->setVrpnServer(m_vrpnServerThread);
	m_actionHandler->setPatientDataCenter(m_patientDataCenter);
	m_actionHandler->setSessionNotesLauncher(m_sessionNotesLauncher);

	createStateMap();
	registerPages();
	createVRPNClient();
	createConnections();
	createPageConnections();
	setupAvailableGames();

	// startup

	m_ovAppsRunner->triggerReloadScenarios();

	m_ovAppsRunner->startACQ(); //TODO: check if success

	m_vrpnServerThread->start();
	m_vrpnClientThread->start();

	//init start page
	m_stateManager->goToState(CentWelcomeState);

	// Install ConsoleOutput to menu
	new DebugOutputManager(&researcherWindow, m_ovAppsRunner);

}

CentController::~CentController()
{
	delete m_logicUiFusion;
	m_logicUiFusion = 0;
}

void CentController::onClose()
{
	m_centWindowController->closeMainWindow();
}

void CentController::createVRPNClient()
{
	m_vrpnClientThread = new VRPNClientThread(this);
	m_vrpnClientThread->registerAnalogClient("liveEEG@localhost", m_setupCapPageController, SLOT(onDataReceived(CentData::AnalogData)));
	m_vrpnClientThread->registerAnalogClient("CENTNoiseLevel@localhost", m_setupCapPageController, SLOT(onNoiseLevel(CentData::AnalogData)), true);
	m_vrpnClientThread->registerAnalogClient("liveEEG@localhost", m_gamePageController, SLOT(onDataReceived(const CentData::AnalogData&)));
	m_vrpnClientThread->registerAnalogClient("CENTNoiseLevel@localhost", m_gamePageController, SLOT(onNoiseLevel(CentData::AnalogData)), true);
	m_vrpnClientThread->registerAnalogClient("liveEEG@localhost", m_baselinePageController, SLOT(onDataReceived(CentData::AnalogData)));
	m_vrpnClientThread->registerAnalogClient("CENTNoiseLevel@localhost", m_baselinePageController, SLOT(onNoiseLevel(CentData::AnalogData)), true);
	m_vrpnClientThread->registerDigitalClient("CENTControlStream@localhost", m_gamePageController, SLOT(onUserInput(CentData::DigitalData)));
	m_vrpnClientThread->registerAnalogClient("CENTInGamePowerLevel@localhost", m_gamePageController, SLOT(onPowerSignal(CentData::AnalogData)), true);

	QSettings settings(QSettings::UserScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);
	int capDriverType = static_cast<CentData::CapDrivers>(settings.value(CENT::Settings::CAP_DRIVER_TYPE, CentData::UnknowCapDriver).toInt());
	if(capDriverType == CentData::CapDriver8thChannels)
	{
		m_vrpnClientThread->registerAnalogClient("liveEEG_5_8@localhost", m_setupCapPageController, SLOT(onDataReceived_5_8(CentData::AnalogData)));
		m_vrpnClientThread->registerAnalogClient("liveEEG_5_8@localhost", m_gamePageController, SLOT(onDataReceived_5_8(CentData::AnalogData)));
		m_vrpnClientThread->registerAnalogClient("liveEEG_5_8@localhost", m_baselinePageController, SLOT(onDataReceived_5_8(CentData::AnalogData)));
	}
}

void CentController::createConnections()
{
	CENT::connect(m_centWindowController, SIGNAL(next()), m_stateManager, SLOT(onNext()));
	CENT::connect(m_centWindowController, SIGNAL(back()), m_stateManager, SLOT(onBack()));
	CENT::connect(m_centWindowController, SIGNAL(close()), m_stateManager, SLOT(onClose()));
	CENT::connect(m_centWindowController, SIGNAL(close()), this, SLOT(onClose()));

	CENT::connect(m_patientDataCenter, SIGNAL(currentSessionPathChanged(const QString&)), m_sessionNotesLauncher, SLOT(setNewSessionPath(const QString&)));
}

void CentController::registerAndConnectPageController(CentState state, BaseCentPageController* pageController)
{
	m_logicUiFusion->registerPageController(state,            *pageController);

	CENT::connect(pageController, SIGNAL(goToState(CentState)), m_stateManager, SLOT(goToState(CentState)));
	CENT::connect(pageController, SIGNAL(error(const QString&)), m_errorHandler, SLOT(onError(const QString&)));
	CENT::connect(pageController, SIGNAL(invokeAction(CENT::ActionType, const QVariantList&, IResultReceiver*, CentState)),
		m_actionHandler, SLOT(executeAction(CENT::ActionType, const QVariantList&, IResultReceiver*, CentState)));
	
	CENT::connect(pageController, SIGNAL(invokeAction(CENT::ActionType, const QVariantList&, IResultReceiver*)),
		m_actionHandler, SLOT(executeAction(CENT::ActionType, const QVariantList&, IResultReceiver*)));
	
	CENT::connect(pageController, SIGNAL(invokeAction(CENT::ActionType, const QVariantList&)),
		m_actionHandler, SLOT(executeAction(CENT::ActionType, const QVariantList&)));
}

void CentController::createPageConnections()
{
	CENT::connect(m_setupCapPageController, SIGNAL(baselineMinutesChanged(int)), m_baselinePageController, SLOT(onRecordTimeChanged(int)));

	CENT::connect(m_gameManager, SIGNAL(gamePluginChanged(ICentGamePlugin*, QString)), 
		m_tutorialPageController, SLOT(onGamePluginChanged(ICentGamePlugin*, QString)));

	CENT::connect(m_gameManager, SIGNAL(gamePluginUnloading()), m_gamePageController, SLOT(onReleaseGamePluginWidget()));

	CENT::connect(m_gameManager, SIGNAL(gamePluginChanged(ICentGamePlugin*, QString)), 
		m_gamePageController, SLOT(onGamePluginChanged(ICentGamePlugin*, QString)));

	// Called in GamePageController for now
	//CENT::connect(m_gamePageController, SIGNAL(startGame()), m_summaryController, SLOT(onGameStarted()));
	//CENT::connect(m_gamePageController, SIGNAL(stopGame()), m_summaryController, SLOT(onGameFinished()));

	CENT::connect(m_patientQuestionaryPageController, SIGNAL(questionaryFilled(CentPatient::Data)), m_patientDataCenter, SLOT(onSavePatientData(CentPatient::Data)));
	CENT::connect(m_patientQuestionaryPageController, SIGNAL(goPageBack()), m_stateManager, SLOT(onBack()));

	CENT::connect(m_baselineInfoPageController, SIGNAL(gameTimeoutChanged(int)), m_gamePageController, SLOT(onGameTimeoutSet(int)));

	CENT::connect(m_gamePageController, SIGNAL(gameStarted(CentPatient::GameData)), m_patientDataCenter, SLOT(onSaveCurrentUserGameData(CentPatient::GameData)));
	CENT::connect(m_gamePageController, SIGNAL(gameEnded(CentPatient::SummaryData)), m_patientDataCenter, SLOT(onSaveCurrentGameSummaryData(CentPatient::SummaryData)));

	CENT::connect(m_patientDataCenter, SIGNAL(patientDataSaveSuccess()), m_stateManager, SLOT(onNext()));
	CENT::connect(m_patientDataCenter, SIGNAL(patientDataSaveFailed()), m_patientQuestionaryPageController, SLOT(onPatientDataSaveFailed()));

	CENT::connect(m_patientDataCenter, SIGNAL(currentSessionPathChanged(const QString&)), m_baselineInfoPageController, SLOT(setCurrentSessionPath(const QString&)));
	CENT::connect(m_patientDataCenter, SIGNAL(betaThetaParsed(double, double)), m_baselineInfoPageController, SLOT(setBetaThetaLevels(double, double)));

	CENT::connect(m_sessionFeedbackPageController, SIGNAL(triggerReloadSessionsScore()), m_patientDataCenter, SLOT(triggerReloadSessionsSummary()));

	CENT::connect(m_patientDataCenter, SIGNAL(gameScoresLoaded(QList< QPair<QString, double> >)), m_summaryPageController, SLOT(onSummaryChanged(QList< QPair<QString, double> >)));
	CENT::connect(m_patientDataCenter, SIGNAL(sessionResultsLoaded(QList< QPair<QString, double> >)), m_sessionFeedbackPageController, SLOT(onSummaryChanged(QList< QPair<QString, double> >)));

	CENT::connect(m_ovAppsRunner, SIGNAL(scenarioGroupsLoaded(QStringList)), m_chooseScenarioGroupPageController, SLOT(fillScenarioGroups(QStringList)));
	CENT::connect(m_ovAppsRunner, SIGNAL(currentScenarioGroupChanged(QString)),  m_chooseScenarioGroupPageController, SLOT(setCurrentScenarioGroup(QString)));
	CENT::connect(m_chooseScenarioGroupPageController, SIGNAL(scenarioGroupChanged(const QString&)), m_ovAppsRunner, SLOT(setScenarioGroup(const QString&)));

	CENT::connect(m_finishPageController, SIGNAL(endingSession()), m_patientDataCenter, SLOT(resetSession()));
	CENT::connect(m_finishPageController, SIGNAL(endingSession()), m_gameManager, SLOT(resetSession()));
	CENT::connect(m_finishPageController, SIGNAL(endingSession()), m_baselineInfoPageController, SLOT(resetSession()));
}

void CentController::createStateMap()
{
	QMap<CentState, CentUiState> stateToUi;
	stateToUi.insert(CentNotInitializedState,     UiNotInitialized);
	stateToUi.insert(CentWelcomeState,            UiWelcomeState);
	stateToUi.insert(CentChooseScenarioState,     UiChooseScenarioState);
	stateToUi.insert(CentCapSetupState,           UiCapSetupState);
	stateToUi.insert(CentBaseLineState,           UiBaseLineState);
	stateToUi.insert(CentBaseLineInfoState,       UiBaseLineInfoState);
	stateToUi.insert(CentGameSelectState,         UiGameSelectState);
	stateToUi.insert(CentTutorialState,           UiTutorialState);
	stateToUi.insert(CentGameState,               UiGameState);
	stateToUi.insert(CentSummaryState,            UiSummaryState);
	stateToUi.insert(CentEvaluationState,         UiEvaluationState);
	stateToUi.insert(CentPatientQuestionaryState, UiPatientQuestionaryState);
	m_logicUiFusion->setStateToUiMap(stateToUi);
}

void CentController::registerPages()
{
	//TODO: organize the lines in the order of correct app flow
	registerAndConnectPageController(CentWelcomeState,            m_welcomePageController);
	registerAndConnectPageController(CentChooseScenarioState,     m_chooseScenarioGroupPageController);
	registerAndConnectPageController(CentCapSetupState,           m_setupCapPageController);
	registerAndConnectPageController(CentBaseLineState,           m_baselinePageController);
	registerAndConnectPageController(CentBaseLineInfoState,       m_baselineInfoPageController);
	registerAndConnectPageController(CentFinishState,             m_finishPageController);
	registerAndConnectPageController(CentGameSelectState,         m_gameSelectPageController);
	registerAndConnectPageController(CentTutorialState,           m_tutorialPageController);
	registerAndConnectPageController(CentGameState,               m_gamePageController);
	registerAndConnectPageController(CentSummaryState,            m_summaryPageController);
	registerAndConnectPageController(CentEvaluationState,         m_evaluationPageController);
	registerAndConnectPageController(CentFeedbackState,           m_sessionFeedbackPageController);
	registerAndConnectPageController(CentPatientQuestionaryState, m_patientQuestionaryPageController);
}

void CentController::setupAvailableGames()
{
	m_gameSelectPageController->setAvailableGames(m_gameManager->availableGames());
}
