#include "MoodInfoWidget.h"

#include "connect.h"
#include "CentPatientData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

namespace
{
	const QString MOOD_ICON_PREFIX(":/images/PatientUI/mood");
	const QString ICON_POSTFIX(".png");

	const char* MOOD_QUESTION_TEXT("What is Your mood today? (Choose one of the characters):");

	const QString BUTTON_NAME_PREFIX = "MoodButton_";
	const QString MOOD_NAME_Group = "MoodButtonGroup";

	const int BUTTON_WIDTH = 65;
	const int BUTTON_HEIGHT = 60;

	const int MOOD_BUTTONS_NUM = 9;
}

MoodInfoWidget::MoodInfoWidget(QWidget* parent)
	: MultiButtonWidgetBase(MOOD_BUTTONS_NUM, parent)
{
	createLayout();
	createConnections();

	QStringList names;
	for(int i = 1; i <= MOOD_BUTTONS_NUM; i++)
	{
		names += BUTTON_NAME_PREFIX + QString::number(i);
	}
	setButtonNames(names);
	setButtonsGroupProperty(MOOD_NAME_Group);
}

MoodInfoWidget::~MoodInfoWidget()
{
}

void MoodInfoWidget::getQuestionaryData(CentPatient::Data& data)
{
	data.m_mood = selection() + 1;
}

void MoodInfoWidget::reset()
{
	MultiButtonWidgetBase::resetSelection();
}

void MoodInfoWidget::triggerValidityCheck()
{
	emitIsValid();
}

bool MoodInfoWidget::checkValid()
{
	return selection() != NOT_SELECTED;
}

void MoodInfoWidget::createLayout()
{

	QLabel* moodLabel = new QLabel(tr(MOOD_QUESTION_TEXT));

	//create mood selection box
	QHBoxLayout* moodButtonsLayout = new QHBoxLayout;
	createButtonsLayout(*moodButtonsLayout);

	QVBoxLayout* moodInfoLayout = new QVBoxLayout;
	moodInfoLayout->addWidget(moodLabel);
	moodInfoLayout->addLayout(moodButtonsLayout);
	moodInfoLayout->addWidget(new QLabel(" ")); //just to keep layout consistent among others

	setLayout(moodInfoLayout);
}

void MoodInfoWidget::createConnections()
{
	MultiButtonWidgetBase::createConnections();
}

void MoodInfoWidget::decorateButton(int buttonNumber, QPushButton& button)
{
	button.setIconSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	button.setIcon(QIcon(MOOD_ICON_PREFIX + QString::number(buttonNumber + 1) + ICON_POSTFIX));

}
