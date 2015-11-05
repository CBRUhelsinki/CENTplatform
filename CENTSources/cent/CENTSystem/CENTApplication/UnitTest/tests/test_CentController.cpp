#include "test_CentController.h"

#include "CentController.h"
#include <QSettings>

void test_CentController::construction()
{
	QSettings settings;
	delete new CentController(settings, m_researcherMainWindow, m_patientMainWindow);
	CentController controller(settings, m_researcherMainWindow, m_patientMainWindow);
}
