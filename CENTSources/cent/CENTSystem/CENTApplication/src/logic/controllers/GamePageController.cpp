#include "GamePageController.h"
#include "ICentGamePlugin.h"
#include "SummaryController.h"

#include "Connect.h"
#include "GamePage.h"
#include "OpenVibeConstants.h"
#include "SetupCapPage.h"

#include <QDebug>
#include <QTimer>
#include <QSettings>
#include <QApplication>

namespace
{
	const QString GAME_CONFIG_FILE_NAME_FORMAT = "plugins/%1.ini"; //plugins/[game_name].ini
	const QString GAME_DURATION_TIME_SETTING   = "GameDurationTime";
	const int UNDEFINED_GAME_TIME = -1;
	const int RELEASE_WIDGET_RETRIES = 30; // up to 3 seconds
	const int RELEASE_WIDGET_TIMEOUT = 100; // 100ms
}

GamePageController::GamePageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, SummaryController& summaryController, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_currentGame(0)
	, m_patientGamePage(0)
	, m_researcherPage(0)
	, m_summaryController(summaryController)
	, m_gameTimeoutTimer(new QTimer(this))
	, m_gameStarted(false)
	, m_manualStop(false)
	, m_gameTimeoutMinutes(UNDEFINED_GAME_TIME)
{
	CENT::connect(this, SIGNAL(userInput(const CentData::DigitalData&)),
		&m_summaryController, SLOT(onUserInput(const CentData::DigitalData&)));

	m_gameTimeoutTimer->setSingleShot(true);
	m_gameTimeoutTimer->setInterval(UNDEFINED_GAME_TIME);
	CENT::connect(m_gameTimeoutTimer, SIGNAL(timeout()), this, SLOT(onEndGame()));
}

GamePageController::~GamePageController()
{
	if(! m_currentGameWidget.isNull())
	{
		onReleaseGamePluginWidget();
	}

	int retries = RELEASE_WIDGET_RETRIES;
	while(! m_currentGameWidget.isNull() && retries > 0)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, RELEASE_WIDGET_TIMEOUT);
		retries--;
	}

	if(! m_currentGameWidget.isNull())
	{
		qCritical() << "Game plugin did not release game widget within defined timeout of 3s, contact game provider for support.";
	}
}

bool GamePageController::activatePage()
{
	m_gameStarted = false;
	m_manualStop = false;
	if(!m_patientGamePage || !m_researcherPage)
	{
		qCritical() << __FUNCTION__ << "No page to activate";
		return false;
	}
	BaseCentPageController::activatePage();
	QVariantList params;
	params.append(OPENVIBE::Game);
	emit invokeAction(CENT::RunOVDesigner, params, this, ControlStateUndefined);

	if (m_currentGame)
	{
		m_currentGame->onStartGame();
		CentData::DigitalData expectedData;
		expectedData.state = 1;
		m_currentGame->onExpectedInput(expectedData);
	}

	while(! m_gameStarted)
	{
		qApp->processEvents(QEventLoop::AllEvents, 100);
	}

	if(m_gameTimeoutTimer->interval() > 0)
	{
		m_gameTimeoutTimer->start();
	}

	return true;
}

bool GamePageController::deactivatePage()
{
	if (m_currentGame)
	{
		m_currentGame->onEndGame();
	}

	emit invokeAction(CENT::StopOVDesigner, QVariantList(), NULL, ControlStateUndefined);
	if(m_gameTimeoutTimer->isActive())
	{
		m_gameTimeoutTimer->stop();
	}

	BaseCentPageController::deactivatePage();

	while(m_gameStarted)
	{
		qApp->processEvents(QEventLoop::AllEvents, 100);
	}

	return true;
}

void GamePageController::result(int err)
{
	if(err != 0)
	{
		QString errText  = tr("Failed to start OpenVibe Designer. Error code: %1").arg(err);
		emit error(errText);
	}
}

void GamePageController::onUserInput(const CentData::DigitalData& data)
{
	emit userInput(data);
}

void GamePageController::onPowerSignal(const CentData::AnalogData& data)
{
	emit powerSignal(data);
}

void GamePageController::onReleaseGamePluginWidget()
{
	if(m_currentGame)
	{
		CENT::disconnect(this, SIGNAL(userInput(CentData::DigitalData)), m_currentGame, SLOT(onUserInput(CentData::DigitalData)));
		CENT::disconnect(this, SIGNAL(powerSignal(CentData::AnalogData)), m_currentGame, SLOT(onPowerSignal(CentData::AnalogData)));
		CENT::disconnect(m_currentGame.data(), SIGNAL(gameEnded()), this, SLOT(onGameEnded()));
		CENT::disconnect(m_currentGame.data(), SIGNAL(gameStarted()), this, SLOT(onGameStarted()));
		m_currentGame->releaseWidget();
	}
}

void GamePageController::onGamePluginChanged(ICentGamePlugin* gamePlugin, QString /*fileName*/)
{
	if(! gamePlugin)
	{
		return;
	}

	m_currentGameData.m_gameName = gamePlugin->gameName();
	m_gameTimeoutTimer->stop();

	m_currentGame = gamePlugin;
	m_currentGameWidget = m_currentGame->gameWidget();
	m_patientGamePage->setGameWidget(m_currentGameWidget);

	CENT::connect(this, SIGNAL(userInput(CentData::DigitalData)), m_currentGame, SLOT(onUserInput(CentData::DigitalData)));
	CENT::connect(this, SIGNAL(powerSignal(CentData::AnalogData)), m_currentGame, SLOT(onPowerSignal(CentData::AnalogData)));
	CENT::connect(m_currentGame, SIGNAL(gameEnded()), this, SLOT(onGameEnded()));
	CENT::connect(m_currentGame, SIGNAL(gameStarted()), this, SLOT(onGameStarted()));
}

void GamePageController::onGameTimeoutSet(int minutes)
{
	m_gameTimeoutMinutes = minutes;
	m_gameTimeoutTimer->setInterval(m_gameTimeoutMinutes * 60 * 1000);
	m_patientGamePage->setGameTimeoutMinutes(m_gameTimeoutMinutes);
}

void GamePageController::onEndGame()
{
	onGameEnded();
	m_manualStop = true; // this is here to prevent onGameEnded(); from being called twice
	m_currentGame->onEndGame();

	emit goToState(CentSummaryState);//this is valid until game internall summary is implemented
}

ICentPage* GamePageController::createResearcherPage()
{
	m_researcherPage = new SetupCapPage(tr("Game Signal"),  SetupCapPage::GameResearcher);
	CENT::connect(m_researcherPage, SIGNAL(endClicked()), this, SLOT(onEndGame()));
	return m_researcherPage;
}

ICentPage* GamePageController::createPatientPage()
{
	m_patientGamePage = new GamePage();

	CENT::connect(this, SIGNAL(userInput(const CentData::DigitalData&)),
	m_patientGamePage, SLOT(onUserInput(const CentData::DigitalData&)));

	CENT::connect(this, SIGNAL(powerSignal(CentData::AnalogData)),
	m_patientGamePage, SLOT(onPowerSignal(CentData::AnalogData)));

	return m_patientGamePage;
}

void GamePageController::onGameStarted()
{
	m_summaryController.onGameStarted();
	CentData::DigitalData data;
	data.state = 1;
	m_summaryController.onExpectedInput(data);

	m_gameStarted = true;
	m_currentGameData.m_gameDate = QDateTime::currentDateTime();
	emit gameStarted(m_currentGameData);
}

void GamePageController::onGameEnded()
{
	if(!m_manualStop)
	{
		m_patientGamePage->onGameEnded();

		m_summaryController.onGameFinished();
		CentPatient::SummaryData currentSummaryData(m_summaryController.score());

		m_gameStarted = false;

		emit gameEnded(currentSummaryData);
	}
}

void GamePageController::onDataReceived(const CentData::AnalogData& data)
{
	if(m_gameStarted)
	{
		m_researcherPage->onDataReceived(data);
		// disabled for now, as no game actually uses it
		//m_currentGame->onEEG(data);
	}
}

void GamePageController::onDataReceived_5_8(const CentData::AnalogData& data)
{
	if(m_gameStarted)
	{
		m_researcherPage->onDataReceived_5_8(data);
	}
}

void GamePageController::onNoiseLevel(const CentData::AnalogData& data)
{
	if(m_gameStarted)
	{
		m_researcherPage->onNoiseLevel(data);
	}
}
