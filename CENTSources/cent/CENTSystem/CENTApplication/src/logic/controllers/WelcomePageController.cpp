#include "WelcomePageController.h"

#include "Connect.h"
#include "SimplePage.h"

namespace
{
	const char* WELCOME_TEXT = "Welcome to CENT - Computer Enabled Neuroplasticity Treatment";
	const int START_BUTTON_ID = 0;
}

WelcomePageController::WelcomePageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
{
}

WelcomePageController::~WelcomePageController()
{
}

ICentPage* WelcomePageController::createResearcherPage()
{
	return createWelcomePage();
}

ICentPage* WelcomePageController::createPatientPage()
{
	return createWelcomePage();
}

ICentPage* WelcomePageController::createWelcomePage()
{
	SimplePage* page = new SimplePage("");
	page->setObjectName("WelcomePage");
	
	page->setFooterText(tr(WELCOME_TEXT));

	page->setBtnText(START_BUTTON_ID, tr("Start"));
	page->setBtnVisible(START_BUTTON_ID, true);
	
	CENT::connect(page, SIGNAL(buttonClicked(int)), this, SLOT(onButtonClicked(int)));
	
	return page;
}

void WelcomePageController::onButtonClicked(int id)
{
	if(id == START_BUTTON_ID)
	{
		emit goToState(CentPatientQuestionaryState);
	}
}
