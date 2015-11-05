#include "FinishPageController.h"

#include "Connect.h"
#include "SimplePage.h"
#include "StateEnums.h"

namespace
{
	const int CLOSE_BUTTON_INDEX = 0;
}

FinishPageController::FinishPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
{

}

FinishPageController::~FinishPageController()
{
}

void FinishPageController::buttonClicked(int num)
{
	if(num == CLOSE_BUTTON_INDEX)
	{
		emit invokeAction(CENT::EndSession, QVariantList(), this, CentWelcomeState);
	}
}

ICentPage* FinishPageController::createResearcherPage()
{
	return createSimplePage();
}

ICentPage* FinishPageController::createPatientPage()
{
	return createSimplePage();
}

SimplePage* FinishPageController::createSimplePage()
{
	SimplePage* page = new SimplePage("");
	page->setObjectName("FinishPage");
	
	page->setBtnText(CLOSE_BUTTON_INDEX, tr("End session"));
	page->setBtnVisible(CLOSE_BUTTON_INDEX, true);

	page->setFooterText(tr("Thank you."));

	CENT::connect(page, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	return page;
}

void FinishPageController::result(int err)
{
	if(err != 0)
	{
		QString errText = QString(tr("Please close any open Session notes before ending session."));
		emit error(errText);
	}
	else
	{
		emit endingSession();
	}
}
