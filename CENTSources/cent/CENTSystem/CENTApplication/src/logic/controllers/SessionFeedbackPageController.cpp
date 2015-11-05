#include "SessionFeedbackPageController.h"

#include "Connect.h"
#include "SessionFeedbackPage.h"
#include "StateEnums.h"

SessionFeedbackPageController::SessionFeedbackPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_patientPage(0)
	, m_researcherPage(0)
{
}

SessionFeedbackPageController::~SessionFeedbackPageController()
{
}

void SessionFeedbackPageController::result(int err)
{
	Q_UNUSED(err);
}

bool SessionFeedbackPageController::activatePage()
{
	emit triggerReloadSessionsScore();
	return BaseCentPageController::activatePage();
}

void SessionFeedbackPageController::onSummaryChanged(const QList< QPair<QString, double>>& data)
{
	m_researcherPage->setData(data);
	m_patientPage->setData(data);

	double score = data.count() > 0 ? data.last().second : -1;
	m_researcherPage->setScore(score);
	m_patientPage->setScore(score);
}

void SessionFeedbackPageController::onNext()
{
	emit goToState(CentEvaluationState);
}

ICentPage* SessionFeedbackPageController::createResearcherPage()
{
	m_researcherPage = createSessionFeedbackPage();
	return m_researcherPage;
}

ICentPage* SessionFeedbackPageController::createPatientPage()
{
	m_patientPage = createSessionFeedbackPage();
	return m_patientPage;
}

SessionFeedbackPage* SessionFeedbackPageController::createSessionFeedbackPage()
{
	SessionFeedbackPage* page = new SessionFeedbackPage();
	page->setScoreLabelText(tr("This time you scored: "));
	CENT::connect(page, SIGNAL(nextClicked()), this, SLOT(onNext()));

	return page;
}
