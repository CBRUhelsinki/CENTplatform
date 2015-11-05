#include "MotivationInfoWidget.h"
#include "connect.h"
#include "CentPatientData.h"

#include <QVboxLayout>
#include <QHboxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QSignalMapper>

namespace
{
	const char* HOW_ARE_YOU_MOTIVATED_TEXT("How motivated do You feel to do neurofeedback today:");
	const char* LOWLY_MOTIVATED_TEXT("Not at all motivated");
	const char* HIGHLY_MOTIVATED_TEXT("Very motivated");

	const QString BUTTON_NAME_PREFIX = "MotivationButton_";
	const QString BUTTON_NAME_GROUP = "MotivationButtonGroup";
	
	const int BUTTON_WIDTH = 65;
	const int BUTTON_HEIGHT = 60;

	const int MOTIVATION_BUTTONS_NUM = 9;
}

MotivationInfoWidget::MotivationInfoWidget(QWidget* parent)
	: MultiButtonWidgetBase(MOTIVATION_BUTTONS_NUM, parent)
{
	createLayout();
	createConnections();

	QStringList names;
	for(int i = 1; i <= MOTIVATION_BUTTONS_NUM; i++)
	{
		names += BUTTON_NAME_PREFIX + QString::number(i);
	}
	setButtonNames(names);
	setButtonsGroupProperty(BUTTON_NAME_GROUP);
}

MotivationInfoWidget::~MotivationInfoWidget()
{
}

void MotivationInfoWidget::getQuestionaryData(CentPatient::Data& data)
{
	data.m_motivation = selection() + 1;
}

void MotivationInfoWidget::reset()
{
	MultiButtonWidgetBase::resetSelection();
}

void MotivationInfoWidget::triggerValidityCheck()
{
	emitIsValid();
}

bool MotivationInfoWidget::checkValid()
{
	return selection() != NOT_SELECTED;
}

void MotivationInfoWidget::createLayout()
{
	QLabel* motivationQuestionText = new QLabel(tr(HOW_ARE_YOU_MOTIVATED_TEXT));
	QLabel* lowMotivationText = new QLabel(tr(LOWLY_MOTIVATED_TEXT));
	QLabel* highMotivationText = new QLabel(tr(HIGHLY_MOTIVATED_TEXT));

	QHBoxLayout* horizontalLayoutButtons = new QHBoxLayout;
	createButtonsLayout(*horizontalLayoutButtons);

	QHBoxLayout* horizontalLayoutScale = new QHBoxLayout;
	horizontalLayoutScale->addWidget(lowMotivationText);
	horizontalLayoutScale->addStretch();
	horizontalLayoutScale->addWidget(highMotivationText);

	QVBoxLayout* verticalLayout = new QVBoxLayout;
	verticalLayout->addWidget(motivationQuestionText);
	verticalLayout->addLayout(horizontalLayoutButtons);
	verticalLayout->addLayout(horizontalLayoutScale);

	setLayout(verticalLayout);
}

void MotivationInfoWidget::createConnections()
{
	MultiButtonWidgetBase::createConnections();
}

void MotivationInfoWidget::decorateButton(int buttonNumber, QPushButton& button)
{
	button.setMinimumWidth(BUTTON_WIDTH);
	button.setMinimumHeight(BUTTON_HEIGHT);
	button.setText(QString::number(buttonNumber + 1));
}
