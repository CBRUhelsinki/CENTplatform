#include "test_CentStateManager.h"
#include "CentStateManager.h"

#include <QTest>

class CentStateManager_testable: public CentStateManager
{
public:
	explicit CentStateManager_testable(LogicUiFusion<CentState, CentUiState>& logicUiFusion, QObject* parent = 0)
		: CentStateManager(logicUiFusion, parent)
	{
	}

	using CentStateManager::m_currentState;
	using CentStateManager::setCurrentState;
	using CentStateManager::onNext;
	using CentStateManager::onBack;
};

void test_CentStateManager::construction()
{
	CentStateManager_testable* pCentStateManager = new CentStateManager_testable(m_logicUiFusion);
	delete pCentStateManager;

	CentStateManager_testable centStateManager(m_logicUiFusion);
}

void test_CentStateManager::setState()
{
	CentStateManager_testable centStateManager(m_logicUiFusion);
	QCOMPARE(centStateManager.m_currentState, CentNotInitializedState);

	centStateManager.setCurrentState(CentWelcomeState);
	QCOMPARE(centStateManager.m_currentState, CentWelcomeState);

	// If we try to set CentInvalidState internal state doesn't change
	centStateManager.setCurrentState(CentInvalidState);
	QCOMPARE(centStateManager.m_currentState, CentWelcomeState);
}

void test_CentStateManager::onNext()
{
	CentStateManager_testable centStateManager(m_logicUiFusion);
	
	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentWelcomeState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentPatientQuestionaryState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentChooseScenarioState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentCapSetupState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentBaseLineState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentBaseLineInfoState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentGameSelectState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentTutorialState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentGameState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentSummaryState);

	centStateManager.onNext();
	QCOMPARE(centStateManager.m_currentState, CentTutorialState);
}

void test_CentStateManager::onBack()
{
	CentStateManager_testable centStateManager(m_logicUiFusion);
	centStateManager.setCurrentState(CentSummaryState);

	centStateManager.onBack();
	QCOMPARE(centStateManager.m_currentState, CentGameState);

	centStateManager.onBack();
	QCOMPARE(centStateManager.m_currentState, CentTutorialState);

	centStateManager.onBack();
	QCOMPARE(centStateManager.m_currentState, CentGameSelectState);

	centStateManager.onBack();
	QCOMPARE(centStateManager.m_currentState, CentBaseLineState);

	centStateManager.onBack();
	QCOMPARE(centStateManager.m_currentState, CentCapSetupState);

	centStateManager.onBack();
	QCOMPARE(centStateManager.m_currentState, CentWelcomeState);
}
