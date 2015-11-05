#ifndef TEST_CENT_STATE_MANAGER_H
#define TEST_CENT_STATE_MANAGER_H

#include <QObject>

#include "CentMainWindow.h"
#include "CentMainWindowController.h"
#include "LogicUiFusion.h"
#include "StateEnums.h"

class test_CentStateManager: public QObject
{
	Q_OBJECT
public:
	explicit test_CentStateManager(QObject* parent = 0)
		: QObject(parent)
		, m_researcherMainWindow("Therapist")
		, m_patientMainWindow("Patient")
		, m_mainWindowController(m_researcherMainWindow, m_patientMainWindow)
		, m_logicUiFusion(m_mainWindowController)
	{
	}

public:
	CentMainWindow m_researcherMainWindow;
	CentMainWindow m_patientMainWindow;
	CentMainWindowController m_mainWindowController;
	LogicUiFusion<CentState, CentUiState> m_logicUiFusion;

private slots:
	void construction();
	void setState();
	void onNext();
	void onBack();
};

#endif // TEST_CENT_STATE_MANAGER_H

