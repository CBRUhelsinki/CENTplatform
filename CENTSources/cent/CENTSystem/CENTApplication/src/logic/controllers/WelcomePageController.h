#ifndef WELCOME_PAGE_CONTROLLER_H
#define WELCOME_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"

class ICentPage;
class WelcomePage;

class WelcomePageController: public BaseCentPageController
{
	Q_OBJECT
public:
	explicit WelcomePageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~WelcomePageController();

signals:
	void startClicked();

private slots:
	void onButtonClicked(int id);

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

	virtual ICentPage* createWelcomePage();


private:
	Q_DISABLE_COPY(WelcomePageController);
};

#endif // WELCOME_PAGE_CONTROLLER_H

