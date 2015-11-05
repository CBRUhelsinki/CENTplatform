#ifndef SUMMARY_PAGE_CONTROLLER_H
#define SUMMARY_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"

class ICentPage;
class SummaryPage;
class SummaryController;

class SummaryPageController: public BaseCentPageController
{
	Q_OBJECT
public:
	explicit SummaryPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, SummaryController& summaryController, QObject* parent = 0);
	virtual ~SummaryPageController();

public slots:
	void onSummaryChanged(QList< QPair<QString, double> > data);

signals:
	void playAgain();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private slots:
	void onNextClicked();
	void onParamsChangeRequest();
	void onChangeGameRequest();
	void onPlayAgain();

private:
	SummaryPage* createSummaryPage(bool therapist);

private: // data
	SummaryPage*       m_researcherPage;
	SummaryPage*       m_patientPage;
	SummaryController& m_summaryController;
};
#endif // SUMMARY_PAGE_CONTROLLER_H

