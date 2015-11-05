#ifndef FINISH_PAGE_CONTROLLER_H
#define FINISH_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "IResultReceiver.h"

class SimplePage;

class FinishPageController : public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit FinishPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~FinishPageController();

	void result(int error);

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

signals:
	void endingSession();

private slots:
	void buttonClicked(int num);

private:
	SimplePage* createSimplePage();
};

#endif // FINNISH_PAGE_CONTROLLER_H
