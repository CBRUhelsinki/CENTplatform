#ifndef SESSION_FEEDBACK_PAGE_CONTROLLER_H
#define SESSION_FEEDBACK_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "IResultReceiver.h"

class SessionFeedbackPage;

class SessionFeedbackPageController : public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit SessionFeedbackPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~SessionFeedbackPageController();

	virtual void result(int err);

	virtual bool activatePage();

public slots:
	void onSummaryChanged(const QList< QPair<QString, double>>& data);

signals:
	void triggerReloadSessionsScore();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private slots:
	void onNext();

private:
	SessionFeedbackPage* createSessionFeedbackPage();

private:
	SessionFeedbackPage* m_researcherPage;
	SessionFeedbackPage* m_patientPage;
};
#endif // SESSION_FEEDBACK_PAGE_CONTROLLER_H
