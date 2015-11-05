#ifndef CHOOSE_SCENARIO_GROUP_PAGE_CONTROLLER_H
#define CHOOSE_SCENARIO_GROUP_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "StateEnums.h"

class CentMainWindow;
class ChooseScenarioGroupPage;

class ChooseScenarioGroupPageController : public BaseCentPageController
{
	Q_OBJECT
public:
	explicit ChooseScenarioGroupPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~ChooseScenarioGroupPageController();

public slots:
	void fillScenarioGroups(const QStringList& names);
	void setCurrentScenarioGroup(const QString& name);

signals:
	void scenarioGroupChanged(const QString& group);

private slots:
	void onStartClicked();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private:
	ChooseScenarioGroupPage* m_researcherPage;
};
#endif // CHOOSE_SCENARIO_GROUP_PAGE_CONTROLLER_H