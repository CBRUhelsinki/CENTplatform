#include "SleepInfoWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

#include "connect.h"
#include "CentPatientData.h"
#include "DataModelCommon.h"

namespace
{
	const char* HOURS_SLEPT_TEXT("Hours of sleep last night:");
	const char* HOURS_SINCE_SLEEP_TEXT("Hours since last sleep:");

	const int MAX_HOURS_SLEPT = 24;
	const int MIN_HOURS_SLEPT = 1;

	const int MAX_HOURS_SINCE_LAST_SLEEP = 72;
	const int MIN_HOURS_SINCE_LAST_SLEEP = 1;
}

SleepInfoWidget::SleepInfoWidget(QWidget* parent)
	: QWidget(parent)
	, m_hoursSleptSpinBox(0)
	, m_hoursSinceSleepSpinBox(0)
{
	setObjectName("SleepInfoWidget");
	createLayout();
	createConnections();
	emit isValid(true);
}

SleepInfoWidget::~SleepInfoWidget()
{

}

void SleepInfoWidget::getQuestionaryData(CentPatient::Data& data)
{
	data.m_hoursSinceLastSleep = m_hoursSinceSleepSpinBox->value();
	data.m_hoursSlept = m_hoursSleptSpinBox->value();
}

void SleepInfoWidget::reset()
{
	m_hoursSleptSpinBox->setValue(0);
	m_hoursSinceSleepSpinBox->setValue(0);
	emit isValid(true);
}

void SleepInfoWidget::triggerValidityCheck()
{
	emit isValid(checkValid());
}

bool SleepInfoWidget::checkValid()
{
	// Spin box values are always valid, we don't need to change the valid state due to reset
	return true;
}

void SleepInfoWidget::createConnections()
{
}

void SleepInfoWidget::createLayout()
{
	m_hoursSleptSpinBox = new QSpinBox;
	m_hoursSleptSpinBox->setObjectName(DataModel::ObjectNames::QUESTIONARY_SLEEP_DURATION);

	m_hoursSinceSleepSpinBox = new QSpinBox;
	m_hoursSinceSleepSpinBox->setObjectName(DataModel::ObjectNames::QUESTIONARY_SLEEP_GAP);

	m_hoursSleptSpinBox->setRange(MIN_HOURS_SLEPT, MAX_HOURS_SLEPT);
	m_hoursSinceSleepSpinBox->setRange(MIN_HOURS_SINCE_LAST_SLEEP, MAX_HOURS_SINCE_LAST_SLEEP);

	m_hoursSleptSpinBox->setButtonSymbols(QSpinBox::NoButtons);
	m_hoursSinceSleepSpinBox->setButtonSymbols(QSpinBox::NoButtons);

	m_hoursSleptSpinBox->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
	m_hoursSinceSleepSpinBox->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));

	QVBoxLayout* sleepLayout = new QVBoxLayout;
	sleepLayout->addStretch();
	sleepLayout->addWidget(new QLabel(tr(HOURS_SLEPT_TEXT)));
	sleepLayout->addWidget(m_hoursSleptSpinBox);
	sleepLayout->addWidget(new QLabel(tr(HOURS_SINCE_SLEEP_TEXT)));
	sleepLayout->addWidget(m_hoursSinceSleepSpinBox);
	sleepLayout->addStretch();

	QLabel* imageLabel = new QLabel;
	imageLabel->setObjectName("imageLabel");
	QVBoxLayout* imageLayout = new QVBoxLayout;
	imageLayout->addStretch();
	imageLayout->addWidget(imageLabel);
	imageLayout->addStretch();

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addLayout(imageLayout);
	layout->addStretch();
	layout->addLayout(sleepLayout);

	setLayout(layout);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}
