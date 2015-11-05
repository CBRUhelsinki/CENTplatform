#ifndef CHOOSE_SCENARIO_GROUP_PAGE_H
#define CHOOSE_SCENARIO_GROUP_PAGE_H

#include "ICentPage.h"

class QComboBox;
class QStringListModel;

class ChooseScenarioGroupPage : public ICentPage
{
	Q_OBJECT
public:
	explicit ChooseScenarioGroupPage(QWidget* parent = 0);
	virtual ~ChooseScenarioGroupPage();

	virtual void activate() {};
	virtual void deactivate() {};

	virtual QString headerText();

public slots:
	void fillScenarioGroups(const QStringList& names);
	void setCurrentScenarioGroup(const QString& name);

signals:
	void scenarioGroupChanged(const QString& group);
	void startClicked();

private:
	void createLayout();

private:
	QComboBox*        m_scenariosCombo;
	QStringListModel* m_scenariosModel;
};

#endif // CHOOSE_SCENARIO_GROUP_PAGE_H
