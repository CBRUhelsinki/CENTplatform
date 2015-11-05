#include "TutorialPageController.h"

#include "Connect.h"
#include "ICentGamePlugin.h"
#include "StateEnums.h"
#include "TutorialPage.h"

TutorialPageController::TutorialPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_researcherTutorialPage(0)
	, m_patientTutorialPage(0)
{

}

TutorialPageController::~TutorialPageController()
{
}

void TutorialPageController::onGamePluginChanged(ICentGamePlugin* gamePlugin, QString fileName)
{
	if (gamePlugin)
	{
		setTutorial(fileName);
	}
}

ICentPage* TutorialPageController::createResearcherPage()
{
	m_researcherTutorialPage = createTutorialPage();
	return m_researcherTutorialPage;
}

ICentPage* TutorialPageController::createPatientPage()
{
	m_patientTutorialPage = createTutorialPage();
	return m_patientTutorialPage;
}

void TutorialPageController::onBackClicked()
{
	emit goToState(CentGameSelectState);
}

void TutorialPageController::onNextClicked()
{
	emit goToState(CentGameState);
}

TutorialPage* TutorialPageController::createTutorialPage()
{
	TutorialPage* tutorialPage = new TutorialPage();
	CENT::connect(tutorialPage, SIGNAL(backClicked()), this, SLOT(onBackClicked()));
	CENT::connect(tutorialPage, SIGNAL(nextClicked()), this, SLOT(onNextClicked()));
	return tutorialPage;
}

void TutorialPageController::setTutorial(const QString& fileName)
{
	if(!m_researcherTutorialPage || !m_patientTutorialPage)
	{
		return;
	}
	m_researcherTutorialPage->setTutorial(fileName);
	m_patientTutorialPage->setTutorial(fileName);
}

