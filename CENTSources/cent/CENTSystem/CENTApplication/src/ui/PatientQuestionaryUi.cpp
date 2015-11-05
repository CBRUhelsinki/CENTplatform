#include "PatientQuestionaryUI.h"

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
#include "ExcitementInfoWidget.h"
#include "IInfoWidget.h"
#include "NameSurnameInfoWidget.h"
#include "SleepInfoWidget.h"
#include "MotivationInfoWidget.h"
#include "MoodInfoWidget.h"
#include "UiHelpers.h"

namespace
{
	const char* START_SESSION_TEXT("Start session");
	const char* CONTINUE_TEXT("Continue");
	const char* BACK_TEXT("Back");

	const QString EXCITEMENT_OBJECT = DataModel::ObjectNames::QUESTIONARY_PATIENT_EXCITEMENT;
	const QString MOTIVATION_OBJECT = DataModel::ObjectNames::QUESTIONARY_PATIENT_MOTIVATION;
	const QString MOOD_OBJECT       = DataModel::ObjectNames::QUESTIONARY_PATIENT_MOOD;

	const int NO_PAGE_SELECTED = -1;
}

PatientQuestionaryUI::PatientQuestionaryUI(QWidget* parent)
	: QWidget(parent)
	, m_wizardWidget(0)
	, m_continueButton(0)
{
	createLayout();
	createConnections();
	reset();
}

PatientQuestionaryUI::~PatientQuestionaryUI()
{
}

void PatientQuestionaryUI::reset()
{
	foreach(IInfoWidget* widget, m_pageWidgets)
	{
		widget->reset();
	}

	m_continueButton->setText(tr(CONTINUE_TEXT));

	m_wizardWidget->setCurrentIndex(0); // 0 for first page
}

int PatientQuestionaryUI::currentPage()
{
	if(m_wizardWidget)
	{
		return m_wizardWidget->currentIndex();
	}
	return NO_PAGE_SELECTED;
}
	
void PatientQuestionaryUI::setCurrentPage(int page)
{
	if(!m_wizardWidget)
	{
		return;
	}
	if(page >= 0 && page < m_wizardWidget->count() && m_wizardWidget->currentIndex() != page)
	{
		if((page + 1) == m_wizardWidget->count())
		{
			m_continueButton->setText(tr(START_SESSION_TEXT));
		}
		else
		{
			m_continueButton->setText(tr(CONTINUE_TEXT));
		}
		m_wizardWidget->setCurrentIndex(page);
		m_pageWidgets.at(page)->triggerValidityCheck();
		emit currentPageChanged(page);
	}
}

void PatientQuestionaryUI::goToNextPage()
{
	int nextPageNum = m_wizardWidget->currentIndex() + 1;

	if(nextPageNum == m_wizardWidget->count())
	{
		if(sender() == m_continueButton)
		{
			CentPatient::Data data;
			getQuestionaryData(data);
			emit questionaryFilled(data);
		}
	}
	else
	{
		if(m_pageWidgets.at(m_wizardWidget->currentIndex())->checkValid())
		{
			setCurrentPage(nextPageNum);
		}
	}
}

void PatientQuestionaryUI::getQuestionaryData(CentPatient::Data& data)
{
	foreach(IInfoWidget* widget, m_pageWidgets)
	{
		widget->getQuestionaryData(data);
	}
}

void PatientQuestionaryUI::enableContinue(bool enable)
{
	m_continueButton->setEnabled(enable);
}

void PatientQuestionaryUI::createLayout()
{
	// Create a widget that holds all pages
	m_wizardWidget = new QStackedWidget;
	createWizardPages(m_wizardWidget);

	QVBoxLayout* wizardLayout = new QVBoxLayout;

	// Create continue button
	m_continueButton = new QPushButton(tr(CONTINUE_TEXT));
	QWidget* buttonsWidget = new QWidget;

	// Create back button
	QPushButton* backButton = new QPushButton(tr(BACK_TEXT));
	CENT::connect(backButton, SIGNAL(clicked()), this, SLOT(onBackButtonClicked()));

	QList<QWidget*> buttons;
	buttons << backButton << m_continueButton;
	bool b = UiHelpers::createButtonFooter(*buttonsWidget, buttons, UiHelpers::AllRight);
	Q_ASSERT(b); Q_UNUSED(b);

	// Layout UI
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

void PatientQuestionaryUI::createWizardPages(QStackedWidget* wizard)
{
	//create wizard pages
	m_nameSurnameInfo                      = new NameSurnameInfoWidget(this);
	SleepInfoWidget*             sleepInfo = new SleepInfoWidget(this);
	MotivationInfoWidget*   motivationInfo = new MotivationInfoWidget(this);
	MoodInfoWidget*               moodInfo = new MoodInfoWidget(this);
	ExcitementInfoWidget*   excitementInfo = new ExcitementInfoWidget(this);

	//set object names where DynamicDataModel requires it
	motivationInfo->setObjectName(MOTIVATION_OBJECT);
	moodInfo->setObjectName(MOOD_OBJECT);
	excitementInfo->setObjectName(EXCITEMENT_OBJECT);

	// setup pages connections
	CENT::connect(m_nameSurnameInfo, SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
	CENT::connect(sleepInfo,       SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
	CENT::connect(motivationInfo,  SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
	CENT::connect(moodInfo,        SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));
	CENT::connect(excitementInfo,  SIGNAL(isValid(bool)), this, SLOT(onQuestionaryUiValidChanged(bool)));

	CENT::connect(motivationInfo, SIGNAL(buttonClicked()), this, SLOT(goToNextPage()));
	CENT::connect(moodInfo, SIGNAL(buttonClicked()), this, SLOT(goToNextPage()));
	CENT::connect(excitementInfo, SIGNAL(buttonClicked()), this, SLOT(goToNextPage()));

	// Fill pages list
	m_pageWidgets.append(static_cast<IInfoWidget*>(m_nameSurnameInfo));
	m_pageWidgets.append(static_cast<IInfoWidget*>(sleepInfo));
	m_pageWidgets.append(static_cast<IInfoWidget*>(motivationInfo));
	m_pageWidgets.append(static_cast<IInfoWidget*>(moodInfo));
	m_pageWidgets.append(static_cast<IInfoWidget*>(excitementInfo));

	int i = 0;
	// Fill stack widget
	wizard->insertWidget(i++, static_cast<QWidget*>(m_nameSurnameInfo));
	wizard->insertWidget(i++, static_cast<QWidget*>(sleepInfo));
	wizard->insertWidget(i++, static_cast<QWidget*>(motivationInfo));
	wizard->insertWidget(i++, static_cast<QWidget*>(moodInfo));
	wizard->insertWidget(i++, static_cast<QWidget*>(excitementInfo));
}

void PatientQuestionaryUI::onContinueButtonClicked()
{
	// Prevent multi clicking
	m_continueButton->setEnabled(false);

	if (m_wizardWidget->currentWidget() == m_nameSurnameInfo)
	{
		CentPatient::Data data;
		m_nameSurnameInfo->getQuestionaryData(data);

		emit userExistsCheckRequest(data.m_userName);
	}
	else
	{
		goToNextPage();
	}
}

void PatientQuestionaryUI::onBackButtonClicked()
{
	// Prevent multi clicking is for now forgetable
	int backPageNum = m_wizardWidget->currentIndex() - 1;

	if(backPageNum < 0)
	{
		emit goPageBack();
	}
	else
	{
		setCurrentPage(backPageNum);
	}
}

void PatientQuestionaryUI::onQuestionaryUiValidChanged(bool isValid)
{
	m_continueButton->setEnabled(isValid);
}

void PatientQuestionaryUI::createConnections()
{
	CENT::connect(m_continueButton, SIGNAL(clicked()), this, SLOT(onContinueButtonClicked()));
}
