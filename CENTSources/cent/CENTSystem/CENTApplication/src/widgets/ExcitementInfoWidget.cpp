#include "ExcitementInfoWidget.h"

#include "connect.h"
#include "CentPatientData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

namespace
{
	const QString EXCITEMENT_ICON_PREFIX(":/images/PatientUI/excitement");
	const QString ICON_POSTFIX(".png");

	const char* EXCITEMENT_QUESTION_TEXT("How excited do You feel today? (Choose one of the characters):");

	const QString EXCITEMENT_NAME_PREFIX = "ExcitementButton_";
	const QString EXCITEMENT_NAME_GROUP = "ExcitementButtonGroup";

	const int BUTTON_WIDTH = 65;
	const int BUTTON_HEIGHT = 60;

	const int EXCITEMENT_BUTTONS_NUM = 9;
}

ExcitementInfoWidget::ExcitementInfoWidget(QWidget* parent)
	: MultiButtonWidgetBase(EXCITEMENT_BUTTONS_NUM, parent)
{
	createLayout();
	createConnections();

	QStringList names;
	for(int i = 1; i <= EXCITEMENT_BUTTONS_NUM; i++)
	{
		names += EXCITEMENT_NAME_PREFIX + QString::number(i);
	}
	setButtonNames(names);
	setButtonsGroupProperty(EXCITEMENT_NAME_GROUP);
}

ExcitementInfoWidget::~ExcitementInfoWidget()
{
}

void ExcitementInfoWidget::getQuestionaryData(CentPatient::Data& data)
{
	data.m_excitement = selection() + 1;
}

void ExcitementInfoWidget::reset()
{
	MultiButtonWidgetBase::resetSelection();
}

void ExcitementInfoWidget::triggerValidityCheck()
{
	emitIsValid();
}

bool ExcitementInfoWidget::checkValid()
{
	return selection() != NOT_SELECTED;
}

void ExcitementInfoWidget::createLayout()
{
	//create excitement selection box
	QLabel* excitementLabel = new QLabel(tr(EXCITEMENT_QUESTION_TEXT));
	QHBoxLayout* excitementButtonsLayout = new QHBoxLayout;
	createButtonsLayout(*excitementButtonsLayout);

	QVBoxLayout* excitementInfoLayout = new QVBoxLayout;
	excitementInfoLayout->addWidget(excitementLabel);
	excitementInfoLayout->addLayout(excitementButtonsLayout);
	excitementInfoLayout->addWidget(new QLabel(" "));	//just to keep layout consistent among others
	setLayout(excitementInfoLayout);
}

void ExcitementInfoWidget::createConnections()
{
	MultiButtonWidgetBase::createConnections();
}

void ExcitementInfoWidget::decorateButton(int buttonNumber, QPushButton& button)
{
	button.setIconSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	button.setIcon(QIcon(EXCITEMENT_ICON_PREFIX + QString::number(buttonNumber + 1) + ICON_POSTFIX));
}
