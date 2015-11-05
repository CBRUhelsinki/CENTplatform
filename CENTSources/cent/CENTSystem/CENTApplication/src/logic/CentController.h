#ifndef CENT_CONTROLLER_H
#define CENT_CONTROLLER_H

#include <QObject>
#include "LogicUiFusion.h"
#include "StateEnums.h"
#include "CentPatientData.h"

class QSettings;

class CentActionHandler;
class CentGameManager;
class CentMainWindow;

//page controllers
class CentMainWindowController;
class BaselinePageController;
class BaselineInfoPageController;
class ChooseExistingUserPageController;
class EvaluationPageController;
class GamePageController;
class SetupCapPageController;
class CentStateManager;
class SummaryPageController;
class GameSelectPageController;
class TutorialPageController;
class UserModeSelectionPageController;
class WelcomePageController;
class SessionFeedbackPageController;
class FinishPageController;
class ChooseScenarioGroupPageController;
class PatientQuestionaryPageController;

class SummaryController;
class PatientDataCenter;
class OVAppsRunner;
class ErrorHandler;

class VRPNClientThread;
class VRPNServerThread;

class SessionNotesLauncher;

class CentController: public QObject
{
	Q_OBJECT
public:
	explicit CentController(QSettings& settings, CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~CentController();

private slots:
	void onClose();

private:
	void createVRPNClient();
	void createConnections();
	void registerAndConnectPageController(CentState state, BaseCentPageController* pageController);
	void createPageConnections();
	void createStateMap();
	void registerPages();
	void setupAvailableGames();

private:
	Q_DISABLE_COPY(CentController)
	CentActionHandler*        m_actionHandler;
	CentMainWindow&           m_researcherWindow;
	CentMainWindow&           m_patientWindow;
	QSettings&                m_settings;
	CentMainWindowController* m_centWindowController;
	CentStateManager*         m_stateManager;
	VRPNClientThread*         m_vrpnClientThread;
	VRPNServerThread*         m_vrpnServerThread;
	CentGameManager*          m_gameManager;
	OVAppsRunner*             m_ovAppsRunner;

	LogicUiFusion<CentState, CentUiState>* m_logicUiFusion;

	// Page controllers
	WelcomePageController*             m_welcomePageController;
	SetupCapPageController*            m_setupCapPageController;
	BaselinePageController*            m_baselinePageController;
	BaselineInfoPageController*        m_baselineInfoPageController;
	FinishPageController*              m_finishPageController;
	GameSelectPageController*          m_gameSelectPageController;
	TutorialPageController*            m_tutorialPageController;
	GamePageController*                m_gamePageController;
	SummaryPageController*             m_summaryPageController;
	SummaryController*                 m_summaryController;
	ChooseScenarioGroupPageController* m_chooseScenarioGroupPageController;
	EvaluationPageController*          m_evaluationPageController;
	PatientQuestionaryPageController*  m_patientQuestionaryPageController;
	SessionFeedbackPageController*     m_sessionFeedbackPageController;

	PatientDataCenter*                 m_patientDataCenter;
	ErrorHandler*                      m_errorHandler;

	SessionNotesLauncher*              m_sessionNotesLauncher;
};

#endif // CENT_CONTROLLER_H
