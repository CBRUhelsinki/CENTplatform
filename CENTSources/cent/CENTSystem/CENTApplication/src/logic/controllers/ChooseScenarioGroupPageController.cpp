#include "ChooseScenarioGroupPageController.h"

#include "ChooseScenarioGroupPage.h"
#include "SimplePage.h"
#include "Connect.h"

namespace
{
	const QString PLACE_ELECTRODES_CONTENT = "<html><head></head><body><center><img src=':/images/SetupCap/SetupCap.png' /></center></body></html>";
}

ChooseScenarioGroupPageController::ChooseScenarioGroupPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
		: BaseCentPageController(researcherWindow, patientWindow, parent)
		, m_researcherPage(0)
{
}

ChooseScenarioGroupPageController::~ChooseScenarioGroupPageController()
{
}

void ChooseScenarioGroupPageController::fillScenarioGroups(const QStringList& names)
{
	m_researcherPage->fillScenarioGroups(names);
}

void ChooseScenarioGroupPageController::setCurrentScenarioGroup(const QString& name)
{
	m_researcherPage->setCurrentScenarioGroup(name);
}

void ChooseScenarioGroupPageController::onStartClicked()
{
	emit goToState(CentCapSetupState);
}

ICentPage* ChooseScenarioGroupPageController::createResearcherPage()
{
	m_researcherPage = new ChooseScenarioGroupPage;
	CENT::connect(m_researcherPage, SIGNAL(startClicked()), this, SLOT(onStartClicked()));
	CENT::connect(m_researcherPage, SIGNAL(scenarioGroupChanged(const QString&)), this, SIGNAL(scenarioGroupChanged(const QString&)));
	return m_researcherPage;
}

ICentPage* ChooseScenarioGroupPageController::createPatientPage()
{
	SimplePage* page = new SimplePage(tr("Relax and Focus"));
	page->setContent(PLACE_ELECTRODES_CONTENT, Qt::AlignVCenter);
	return page;
}
