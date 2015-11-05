#include "EvaluationQuestionaryUi.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QStackedWidget>
#include <QPushButton>
#include <QDebug>

#include "Connect.h"
#include "DataModelCommon.h"
#include "MultiButtonWidget.h"
#include "UiHelpers.h"

namespace
{
	const char* CONTINUE_TEXT("Continue");
	const char* SAVE_SESSION_TEXT("Save");
	const char* BACK_TEXT("Back");

	const QString PERFORMANCE_OBJECT = "PerformanceInfoObject";
	const QString STRESS_OBJECT = "StressInfoObject";

	const QString PERFORMANCE_BUTTON_GROUP = "PerformanceButtonGroup";
	const QString STRESS_BUTTON_GROUP = "StressInfoButtonGroup";

	const QString PERFORMANCE_QUESTION = "How hard did you have to work to accomplish your level of performance?";
	const QString STRESS_QUESTION = "How insecure, discouraged, irritated, stressed, and annoyed were you?";

	const QString LOW_TEXT = "Very Low";
	const QString HIGH_TEXT = "Very High";

	const int NO_PAGE_SELECTED = -1;

	const int BUTTONS_COUNT = 9;
}

EvaluationQuestionaryUi::EvaluationQuestionaryUi(QWidget* parent)
	: QWidget(parent)
	, m_wizardWidget(0)
{
	createLayout();
	createConnections();
	reset();
}

EvaluationQuestionaryUi::~EvaluationQuestionaryUi()
{
}

void EvaluationQuestionaryUi::reset()
{
	m_performanceWidget->reset();
	m_stressWidget->reset();

	m_wizardWidget->setCurrentIndex(0); // 0 for first page
}

int EvaluationQuestionaryUi::currentPage()
{
	if(m_wizardWidget)
	{
		return m_wizardWidget->currentIndex();
	}
	return NO_PAGE_SELECTED;
}
	
void EvaluationQuestionaryUi::setCurrentPage(int page)
{
	if(!m_wizardWidget)
	{
		return;
	}
	if(page >= 0 && page < m_wizardWidget->count() && m_wizardWidget->currentIndex() != page)
	{
		m_wizardWidget->setCurrentIndex(page);
		MultiButtonWidget* multiButtonWidget = qobject_cast<MultiButtonWidget*>(m_wizardWidget->widget(page));
		if(multiButtonWidget)
		{
			multiButtonWidget->triggerValidityCheck();
		}

		m_backButton->setEnabled(page != 0);
		emit currentPageChanged(page);
	}
	if(page == m_wizardWidget->count() - 1)
	{
		m_continueButton->setText(tr(SAVE_SESSION_TEXT));
	}
	else
	{
		m_continueButton->setText(tr(CONTINUE_TEXT));
	}
}

void EvaluationQuestionaryUi::goToNextPage()
{
	int nextPageNum = m_wizardWidget->currentIndex() + 1;

	if(nextPageNum == m_wizardWidget->count())
	{
		if(sender() == m_continueButton)
		{
			CentPatient::EvaluationData data;
			getQuestionaryData(data);
			emit questionaryFilled(data);
		}
	}
	else
	{
		MultiButtonWidget* multiButtonWidget = qobject_cast<MultiButtonWidget*>(m_wizardWidget->currentWidget());
		if(multiButtonWidget && multiButtonWidget->selection() != MultiButtonWidgetBase::NOT_SELECTED)
		{
			setCurrentPage(nextPageNum);
		}
	}
}

void EvaluationQuestionaryUi::getQuestionaryData(CentPatient::EvaluationData& data)
{
	data.m_performance = m_performanceWidget->selection() + 1;
	data.m_stress = m_stressWidget->selection() + 1;
}

QString EvaluationQuestionaryUi::headerText()
{
	switch(currentPage())
	{
		case 0:
			return tr("Effort");

		case 1:
			return tr("Frustration");

		default:
			return "";
	}
}

void EvaluationQuestionaryUi::createLayout()
{
	// Create a widget that holds all pages
	m_wizardWidget = new QStackedWidget;
	createWizardPages(m_wizardWidget);

	QVBoxLayout* wizardLayout = new QVBoxLayout;

	// Create continue button
	m_continueButton = new QPushButton(tr(CONTINUE_TEXT));
	QWidget* buttonsWidget = new QWidget;

	// Create back button
	m_backButton = new QPushButton(tr(BACK_TEXT));

	QList<QWidget*> buttons;
	buttons << m_backButton << m_continueButton;
	bool b = UiHelpers::createButtonFooter(*buttonsWidget, buttons, UiHelpers::AllRight);
	Q_ASSERT(b); Q_UNUSED(b);

	// Layout UI
	m_wizardWidget->setContentsMargins(0, 0, 0, 0);
	m_wizardWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

	QSpacerItem* vSpacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
	QSpacerItem* vSpacer2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

	wizardLayout->addSpacerItem(vSpacer1);
	wizardLayout->addWidget(m_wizardWidget);
	wizardLayout->addSpacerItem(vSpacer2);

	wizardLayout->addWidget(buttonsWidget);
	wizardLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(wizardLayout);

	// Set the current visible page
	m_wizardWidget->setCurrentIndex(0); // first page
}

void EvaluationQuestionaryUi::createWizardPages(QStackedWidget* wizard)
{
	//create wizard pages
	m_performanceWidget = new MultiButtonWidget(BUTTONS_COUNT);
	m_performanceWidget->setDescriptionText(PERFORMANCE_QUESTION);
	m_performanceWidget->setLowScaleText(LOW_TEXT);
	m_performanceWidget->setHighScaleText(HIGH_TEXT);
	m_performanceWidget->setButtonsGroupProperty(PERFORMANCE_BUTTON_GROUP);

	m_stressWidget = new MultiButtonWidget(BUTTONS_COUNT);
	m_stressWidget->setDescriptionText(STRESS_QUESTION);
	m_stressWidget->setLowScaleText(LOW_TEXT);
	m_stressWidget->setHighScaleText(HIGH_TEXT);
	m_stressWidget->setButtonsGroupProperty(STRESS_BUTTON_GROUP);

	//set object names where DynamicDataModel requires it
	m_performanceWidget->setObjectName(PERFORMANCE_OBJECT);
	m_stressWidget->setObjectName(STRESS_OBJECT);

	int i = 0;
	// Fill stack widget
	wizard->insertWidget(i++, static_cast<QWidget*>(m_performanceWidget));
	wizard->insertWidget(i++, static_cast<QWidget*>(m_stressWidget));
}

void EvaluationQuestionaryUi::onQuestionaryUiValidChanged(bool isValid)
{
	m_continueButton->setEnabled(isValid);
}

void EvaluationQuestionaryUi::createConnections()
{
	// setup pages connections
	CENT::connect(m_performanceWidget, SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
	CENT::connect(m_stressWidget,      SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));

	// setup bottom buttons connections
	CENT::connect(m_continueButton, SIGNAL(clicked()), this, SLOT(goToNextPage()));
	CENT::connect(m_backButton,     SIGNAL(clicked()), this, SLOT(onBackButtonClicked()));

	CENT::connect(m_performanceWidget, SIGNAL(buttonClicked()), this, SLOT(goToNextPage()));
	CENT::connect(m_stressWidget,      SIGNAL(buttonClicked()), this, SLOT(goToNextPage()));

	CENT::connect(m_performanceWidget, SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
	CENT::connect(m_stressWidget,      SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
}

void EvaluationQuestionaryUi::onContinueButtonClicked()
{
	// Prevent multi clicking
	m_continueButton->setEnabled(false);

	goToNextPage();
}

void EvaluationQuestionaryUi::onBackButtonClicked()
{
	// Prevent multi clicking is for now forgetable
	int backPageNum = m_wizardWidget->currentIndex() - 1;

	if(backPageNum >= 0)
	{
		setCurrentPage(backPageNum);
	}
}
