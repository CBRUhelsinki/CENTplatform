#ifndef TUTORIAL_PAGE_CONTROLLER_H
#define TUTORIAL_PAGE_CONTROLLER_H

#include <QPointer>
#include "BaseCentPageController.h"

class ICentGamePlugin;
class ICentPage;
class TutorialPage;

class TutorialPageController: public BaseCentPageController
{
	Q_OBJECT
public:
	explicit TutorialPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~TutorialPageController();

public slots:
	void onGamePluginChanged(ICentGamePlugin* gamePlugin, QString fileName);
	void onBackClicked();
	void onNextClicked();

signals:
	void tutorialFinished();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

	virtual TutorialPage* createTutorialPage();

private:
	void setTutorial(const QString& fileName);

private:
	Q_DISABLE_COPY(TutorialPageController);

	TutorialPage* m_researcherTutorialPage;
	TutorialPage* m_patientTutorialPage;
};
#endif // TUTORIAL_PAGE_CONTROLLER_H

