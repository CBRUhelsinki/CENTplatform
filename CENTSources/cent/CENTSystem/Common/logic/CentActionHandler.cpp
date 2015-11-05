#include "CentActionHandler.h"

#include <QDebug>

#include "CentGameManager.h"
#include "CentPatientData.h"
#include "CentStateManager.h"
#include "Connect.h"
#include "IResultReceiver.h"
#include "PatientDataCenter.h"
#include "SessionNotesLauncher.h"
#include "VRPNServerThread.h"

CentActionHandler::CentActionHandler(CentStateManager& stateManager,QObject* parent)
	: QObject(parent)
	, m_stateManager(stateManager)
	, m_ovAppsRunner(0)
	, m_currentReceiver(0)
	, m_stateOnSuccess(ControlStateUndefined)
	, m_gameManager(0)
	, m_vrpnServer(0)
	, m_patientDataCenter(0)
	, m_sessionNotesLauncher(0)
{
	m_actionsMap.insert(CENT::RunOVDesigner, ActionData(CENT::ActionConfig::RUN_DESIGNER_PARAM_COUNT, &CentActionHandler::runOVDesigner));
	m_actionsMap.insert(CENT::StopOVDesigner, ActionData(CENT::ActionConfig::TRIGGER_ACTION_PARAM_COUNT, &CentActionHandler::stopOVDesigner));
	m_actionsMap.insert(CENT::SetCurrentGame, ActionData(CENT::ActionConfig::SET_CURRENT_GAME_PARAM_COUNT, &CentActionHandler::setCurrentGame));
	m_actionsMap.insert(CENT::SetStimulationButton, ActionData(CENT::ActionConfig::SET_STIMULATION_BUTTON_PARAM_COUNT, &CentActionHandler::setStimulationButton));
	m_actionsMap.insert(CENT::ParseBetaThetaLevelValues, ActionData(CENT::ActionConfig::TRIGGER_ACTION_PARAM_COUNT, &CentActionHandler::parseBetaThetaLevelValues));
	m_actionsMap.insert(CENT::SaveCoeffValues, ActionData(CENT::ActionConfig::SAVE_COEFF_VALUES_PARAM_COUNT, &CentActionHandler::saveCoeffValues));
	m_actionsMap.insert(CENT::SaveEvaluation, ActionData(CENT::ActionConfig::SAVE_EVALUATION_PARAM_COUNT, &CentActionHandler::saveEvaluation));
	m_actionsMap.insert(CENT::UseDefaultBaseline, ActionData(CENT::ActionConfig::TRIGGER_ACTION_PARAM_COUNT, &CentActionHandler::useDefaultBaseline));
	m_actionsMap.insert(CENT::ShowSettings, ActionData(CENT::ActionConfig::SET_CURRENT_GAME_PARAM_COUNT, &CentActionHandler::showSettings));
	m_actionsMap.insert(CENT::EndSession, ActionData(CENT::ActionConfig::TRIGGER_ACTION_PARAM_COUNT, &CentActionHandler::endSession));
}

CentActionHandler::~CentActionHandler()
{
}


void CentActionHandler::setOVAppsRunner(OVAppsRunner* appsRunner)
{
	if(m_ovAppsRunner)
	{
		CENT::disconnect(m_ovAppsRunner, SIGNAL(startResult(int)), this, SLOT(executionResult(int)));
	}
	m_ovAppsRunner = appsRunner;
	if(m_ovAppsRunner)
	{
		CENT::connect(m_ovAppsRunner, SIGNAL(startResult(int)), this, SLOT(executionResult(int)));
	}
}

void CentActionHandler::setGameManager(CentGameManager* gameManager)
{
	m_gameManager = gameManager;
}

void CentActionHandler::setVrpnServer(VRPNServerThread* vrpnServer)
{
	m_vrpnServer = vrpnServer;
}

void CentActionHandler::setPatientDataCenter(PatientDataCenter* dataCenter)
{
	m_patientDataCenter = dataCenter;
}

void CentActionHandler::setSessionNotesLauncher(SessionNotesLauncher* sessionNotesLauncher)
{
	m_sessionNotesLauncher = sessionNotesLauncher;
}

void CentActionHandler::executeAction(CENT::ActionType actionType, const QVariantList& params, IResultReceiver* invoker, CentState stateOnSuccess)
{
	Q_ASSERT(m_stateOnSuccess == ControlStateUndefined);
	Q_ASSERT(m_currentReceiver == 0);
	bool sync = false;
	int error = Success;
	m_currentReceiver = invoker;
	m_stateOnSuccess = stateOnSuccess;

	int paramCount = m_actionsMap.value(actionType).m_paramCount;
	if(paramCount == -1)
	{
		//TODO: there should be an option to define action with minimal or unknown number of parameters, for example stimulation buttons could then be all set at once
		qCritical() << __FUNCTION__ << "Action not defined in map or defined with negative param count.";
		error = InternalFatal;
		sync = true;
	}
	else
	{
		if(params.count() == paramCount)
		{
			ExecutionMethodPtr f = m_actionsMap.value(actionType).m_executionMethodPtr;
			error = (this->*f)(params);
		}
		else
		{
			qWarning() << __FUNCTION__ << "Param count incorrect.";
			error = ParamCount;
			sync = true;
		}
	}

	if(sync)
	{
		m_stateOnSuccess = ControlStateUndefined;
		m_currentReceiver = 0;
		if(invoker)
		{
			invoker->result(error);
		}
	}
}

void CentActionHandler::executionResult(int error)
{
	if(m_currentReceiver)
	{
		m_currentReceiver->result(error);
	}

	CentState toState = m_stateOnSuccess;
	m_stateOnSuccess = ControlStateUndefined;
	m_currentReceiver = 0;
	if(toState != ControlStateUndefined && error == Success)
	{
		m_stateManager.goToState(toState);
	}
}

int CentActionHandler::runOVDesigner(const QVariantList& params)
{
	if(!m_ovAppsRunner)
	{
		return NoExecutor;
	}
	m_ovAppsRunner->startOV(static_cast<OPENVIBE::DesignerScenarioType>(params.at(0).toInt()));

	return Success;
}

int CentActionHandler::stopOVDesigner(const QVariantList& params)
{
	Q_UNUSED(params);

	if(!m_ovAppsRunner)
	{
		return NoExecutor;
	}

	m_ovAppsRunner->stopOV();

	return Success;
}

int CentActionHandler::setCurrentGame(const QVariantList& params)
{
	QString gameName = params.at(0).toString();
	ErrorCodes errCode = Success;

	if(! m_gameManager)
	{
		errCode = GameManagerUnavailable;
	}
	else if(! m_gameManager->changeCurrentGame(gameName))
	{
		errCode = GameUnavailable;
	}

	executionResult(errCode);

	return errCode;
}

int CentActionHandler::setStimulationButton(const QVariantList& params)
{
	int which = params.at(0).toInt();
	int state = params.at(1).toInt();
	ErrorCodes errCode = Success;

	if(! m_vrpnServer)
	{
		errCode = VrpnServerUnavailable;
	}
	else
	{
		m_vrpnServer->setButtonState(which, state);
	}

	executionResult(errCode);

	return errCode;
}

int CentActionHandler::parseBetaThetaLevelValues(const QVariantList& params)
{
	Q_UNUSED(params);
	ErrorCodes errCode = Success;

	if(! m_patientDataCenter)
	{
		errCode = DataCenterUnavailable;
	}
	else
	{
		m_patientDataCenter->triggerParseBetaTheta();
	}

	executionResult(errCode);
	return errCode;
}

int CentActionHandler::saveCoeffValues(const QVariantList& params)
{
	double beta  = params.at(0).toDouble();
	double theta = params.at(1).toDouble();
	ErrorCodes errCode = Success;

	if(! m_patientDataCenter)
	{
		errCode = DataCenterUnavailable;
	}
	else
	{
		bool b = m_patientDataCenter->saveCoefficients(beta, theta);
		if(!b)
		{
			errCode = WriteConfigFailed;
		}
	}

	executionResult(errCode);
	return errCode;
}

int CentActionHandler::saveEvaluation(const QVariantList& params)
{
	CentPatient::EvaluationData data = params[0].value<CentPatient::EvaluationData>();
	if(data.m_performance == CentPatient::INVALID || data.m_stress == CentPatient::INVALID)
	{
		return ArgumentsError;
	}
	ErrorCodes errCode = Success;

	if(! m_patientDataCenter)
	{
		errCode = DataCenterUnavailable;
	}
	else
	{
		if(!m_patientDataCenter->saveEvaluationData(data))
		{
			errCode = SaveFileFailed;
		}
	}

	executionResult(errCode);
	return errCode;
}

int CentActionHandler::useDefaultBaseline(const QVariantList& params)
{
	Q_UNUSED(params);
	ErrorCodes errCode = Success;

	if(! m_patientDataCenter)
	{
		errCode = DataCenterUnavailable;
	}
	else
	{
		errCode = m_patientDataCenter->useDefaultBaselineFromIEP() ? Success : BaselineCopyError;
	}

	executionResult(errCode);
	return errCode;
}

int CentActionHandler::showSettings(const QVariantList& params)
{
	QString gameName = params.at(0).toString();
	ErrorCodes errCode = Success;

	if(! m_gameManager)
	{
		errCode = GameManagerUnavailable;
	}
	else if(! m_gameManager->showSettings(gameName))
	{
		errCode = GameUnavailable;
	}

	executionResult(errCode);

	return errCode;
}

int CentActionHandler::endSession(const QVariantList& params)
{
	Q_UNUSED(params);
	ErrorCodes errCode = Success;

	if(! m_sessionNotesLauncher)
	{
		errCode = SessionNotesLauncherUnavailable;
	}
	else
	{
		errCode = m_sessionNotesLauncher->canEndSession() ? Success : SessionNotesRunning;
	}

	executionResult(errCode);
	return errCode;
}
