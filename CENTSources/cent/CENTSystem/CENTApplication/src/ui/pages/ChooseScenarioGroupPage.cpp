#include "ChooseScenarioGroupPage.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

#include "Connect.h"
#include "UiHelpers.h"

namespace
{
	const int START_BUTTON_MAX_WIDTH = 100;
}

ChooseScenarioGroupPage::ChooseScenarioGroupPage(QWidget* parent)
	: ICentPage(parent)
	, m_scenariosCombo(0)
{
	createLayout();
}

ChooseScenarioGroupPage::~ChooseScenarioGroupPage()
{
}

QString ChooseScenarioGroupPage::headerText()
{
	return tr("Choose scenario group");
}

void ChooseScenarioGroupPage::fillScenarioGroups(const QStringList& names)
{
	m_scenariosModel->setStringList(names);
}

void ChooseScenarioGroupPage::setCurrentScenarioGroup( const QString& name)
{
	m_scenariosCombo->setCurrentIndex(m_scenariosModel->stringList().indexOf(name));
}

void ChooseScenarioGroupPage::createLayout()
{
	//Welcome texts
	QLabel* welcomeLabel = new QLabel();
	welcomeLabel->setText(tr("Choose scenario group"));
	welcomeLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

	m_scenariosCombo = new QComboBox();
	m_scenariosModel = new QStringListModel(this);
	m_scenariosCombo->setModel(m_scenariosModel);

	CENT::connect(m_scenariosCombo, SIGNAL(currentIndexChanged(const QString &)), this, SIGNAL(scenarioGroupChanged(const QString &)));

	// scenarios combo layout
	QHBoxLayout* comboLayout = new QHBoxLayout;
	comboLayout->addStretch();
	comboLayout->addWidget(m_scenariosCombo);
	comboLayout->addStretch();
	
	//Start button
	QPushButton* startButton = new QPushButton();
	startButton->setText(tr("Start"));
	startButton->setMaximumWidth(START_BUTTON_MAX_WIDTH);
	CENT::connect(startButton, SIGNAL(clicked()), this, SIGNAL(startClicked()));

	//button widget
	QWidget* buttonWidget = new QWidget;
	QList<QWidget*> list;
	list << startButton;

	bool b = UiHelpers::createButtonFooter(*buttonWidget, list, UiHelpers::CenterWidgets);
	Q_ASSERT(b); Q_UNUSED(b);


	//main layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addStretch();
	layout->addWidget(welcomeLabel);
	layout->addLayout(comboLayout);
	layout->addStretch();
	layout->addWidget(buttonWidget);

	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}
