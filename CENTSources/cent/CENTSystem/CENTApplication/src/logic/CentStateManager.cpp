#include "CentStateManager.h"
#include "LogicUiFusion.h"

CentStateManager::CentStateManager(LogicUiFusion<CentState, CentUiState>& logicUiFusion, QObject* parent)
	: QObject(parent)
	, m_currentState(CentNotInitializedState)
	, m_logicUiFusion(logicUiFusion)
{
}

CentStateManager::~CentStateManager()
{
}

void CentStateManager::onNext()
{
	switch (m_currentState)
	{
		case CentNotInitializedState:
			setCurrentState(CentWelcomeState);
			break;

		case CentWelcomeState:
			setCurrentState(CentPatientQuestionaryState);
			break;

		case CentPatientQuestionaryState:
			setCurrentState(CentChooseScenarioState);
			break;

		case CentChooseScenarioState:
			setCurrentState(CentCapSetupState);
			break;

		case CentCapSetupState:
			setCurrentState(CentBaseLineState);
			break;

		case CentBaseLineState:
			setCurrentState(CentBaseLineInfoState);
			break;

		case CentBaseLineInfoState:
			setCurrentState(CentGameSelectState);
			break;

		case CentGameSelectState:
			setCurrentState(CentTutorialState);
			break;

		case CentTutorialState:
			setCurrentState(CentGameState);
			break;

		case CentGameState:
			setCurrentState(CentSummaryState);
			break;

		case CentSummaryState:
			setCurrentState(CentTutorialState);
			break;

		default:
			break;
	}
}

void CentStateManager::onBack()
{
	switch (m_currentState)
	{
		case CentCapSetupState:
			setCurrentState(CentWelcomeState);
			break;

		case CentBaseLineState:
			setCurrentState(CentCapSetupState);
			break;

		case CentBaseLineInfoState:
			setCurrentState(CentChooseScenarioState);
			break;

		case CentGameSelectState:
			setCurrentState(CentBaseLineState);
			break;

		case CentTutorialState:
			setCurrentState(CentGameSelectState);
			break;

		case CentGameState:
			setCurrentState(CentTutorialState);
			break;

		case CentSummaryState:
			setCurrentState(CentGameState);
			break;

		case CentPatientQuestionaryState:
			setCurrentState(CentWelcomeState);
			break;

		default:
			break;
	}
}

void CentStateManager::onClose()
{
}

void CentStateManager::goToState(CentState state)
{
	switch(state)
	{
		case ControlStateUndefined:
		{
			return;
		}
		case ControlStatePrevious:
		{
			onBack();
			break;
		}
		case ControlStateNext:
		{
			onNext();
			break;
		}
		default:
		{
			setCurrentState(state);
			break;
		}
	}
}

void CentStateManager::setCurrentState(CentState state)
{
	if (CentInvalidState == state)
	{
		return;
	}

	if (state == m_currentState)
	{
		return;
	}
	
	m_currentState = state;

	m_logicUiFusion.onStateChanged(m_currentState);
}
