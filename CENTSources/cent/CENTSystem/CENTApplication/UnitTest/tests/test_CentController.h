#ifndef TEST_CENT_CONTROLLER_H
#define TEST_CENT_CONTROLLER_H

#include <QObject>

#include "CentMainWindow.h"
#include "CentMainWindowController.h"
#include "LogicUiFusion.h"
#include "StateEnums.h"

class test_CentController: public QObject
{
	Q_OBJECT
public:
	explicit test_CentController(QObject* parent = 0)
		: QObject(parent)
		, m_patientMainWindow("Patient")
		, m_researcherMainWindow("Therapist")
		, m_mainWindowController(m_researcherMainWindow, m_patientMainWindow)
		, m_logicUiFusion(m_mainWindowController)
	{
	}

public:
	CentMainWindow m_patientMainWindow;
	CentMainWindow m_researcherMainWindow;
	CentMainWindowController m_mainWindowController;
	LogicUiFusion<CentState, CentUiState> m_logicUiFusion;
private slots:
	void construction();
};

#endif // TEST_CENT_CONTROLLER_H

