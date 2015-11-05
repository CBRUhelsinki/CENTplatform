#include "SummaryPageController.h"
#include "SummaryController.h"

#include "Connect.h"
#include "SummaryPage.h"

SummaryPageController::SummaryPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, SummaryController& summaryController, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_summaryController(summaryController)
	, m_researcherPage(0)
	, m_patientPage(0)
{
}

SummaryPageController::~SummaryPageController()
{
}

void SummaryPageController::onSummaryChanged(QList< QPair<QString, double> > data)
{
	m_researcherPage->setData(data);
	m_patientPage->setData(data);

	m_researcherPage->setScore(data.last().second);
	m_patientPage->setScore(data.last().second);
}

ICentPage* SummaryPageController::createResearcherPage()
{
	m_researcherPage = createSummaryPage(true);
	return m_researcherPage;
}

void SummaryPageController::onNextClicked()
{
	emit goToState(CentFeedbackState);
}

void SummaryPageController::onParamsChangeRequest()
{
	emit goToState(CentBaseLineInfoState);
}

void SummaryPageController::onChangeGameRequest()
{
	emit goToState(CentGameSelectState);
}

void SummaryPageController::onPlayAgain()
{
	emit goToState(CentGameState);
}

ICentPage* SummaryPageController::createPatientPage()
{
	m_patientPage = createSummaryPage(false);
	return m_patientPage;
}

SummaryPage* SummaryPageController::createSummaryPage(bool therapist)
{
	SummaryPage::Mode mode = therapist ? SummaryPage::TherapistMode : SummaryPage::PatientMode;
	SummaryPage* page = new SummaryPage(mode);
	page->setScoreLabelText(tr("This time you scored: "));
	CENT::connect(page, SIGNAL(playAgain()), this, SLOT(onPlayAgain()));
	CENT::connect(page, SIGNAL(nextClicked()), this, SLOT(onNextClicked()));
	CENT::connect(page, SIGNAL(changeParams()), this, SLOT(onParamsChangeRequest()));
	CENT::connect(page, SIGNAL(changeGame()), this, SLOT(onChangeGameRequest()));

	return page;
}
