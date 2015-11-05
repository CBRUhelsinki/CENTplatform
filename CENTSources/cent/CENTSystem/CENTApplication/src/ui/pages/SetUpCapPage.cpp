#include "SetupCapPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QSettings>

#include "Connect.h"
#include "EEGMultiWidget.h"
#include "EEGPlot.h"
#include "NoisePlot.h"
#include "UiHelpers.h"
#include "CentConstants.h"

namespace
{
	const char* NOISE_PLOT_LABEL = "Noise level";

	const int BASELINE_TIME_MIN = 1;
	const int BASELINE_TIME_MAX = 10;

	const int BUTTON_MAX_WIDTH = 100;
	const int SPIN_MIN_WIDTH = 30;

	const QString TIMER_DISPLAY_FORMAT = "m:ss";

	unsigned int NUM_CHANNELS_PER_CONTAINER = 4;
}

SetupCapPage::SetupCapPage(const QString& title, PageMode mode, QWidget* parent)
	: m_title(title)
	, ICentPage(parent)
	, m_stateLabel(0)
	, m_endButton(0)
	, m_timeLabel(0)
	, m_timeLeft(0, 0)
	, m_timer(new QTimer(this))
{
	blockSignals(true);

	QSettings settings(QSettings::UserScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);
	m_capDriverType = static_cast<CentData::CapDrivers>(settings.value(CENT::Settings::CAP_DRIVER_TYPE, CentData::UnknowCapDriver).toInt());

	//main layout
	QVBoxLayout* layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);

	// horizontal layout for plots and noise level
	QHBoxLayout* plotsLayout = new QHBoxLayout;

	//refresh scales 1/s
	QTimer* timer = new QTimer(this);
	timer->setInterval(1000); // 1s

	// EEGContainer 1-4
	EEGMultiWidget * EEGContainer = new EEGMultiWidget(this);
	for(unsigned int i = 0; i < NUM_CHANNELS_PER_CONTAINER - 1; i++)
	{
		EEGContainer->addPlot(i, NUM_CHANNELS_PER_CONTAINER, false);
	}
	EEGContainer->addPlot(NUM_CHANNELS_PER_CONTAINER - 1, NUM_CHANNELS_PER_CONTAINER, true);

	EEGContainer->configureLayout();

	//add to layout
	plotsLayout->addWidget(EEGContainer);

	if(m_capDriverType == CentData::CapDriver8thChannels)
	{
		// EEGContainer 5-8
		EEGMultiWidget * EEGContainer_5_8 = new EEGMultiWidget(this);
		for(unsigned int i = 0; i < NUM_CHANNELS_PER_CONTAINER - 1; i++)
		{
			EEGContainer_5_8->addPlot(i, NUM_CHANNELS_PER_CONTAINER, false);
		}
		EEGContainer_5_8->addPlot(NUM_CHANNELS_PER_CONTAINER - 1, NUM_CHANNELS_PER_CONTAINER, true);

		EEGContainer_5_8->configureLayout();

		//add to layout
		plotsLayout->addWidget(EEGContainer_5_8);

		CENT::connect(timer, SIGNAL(timeout()), EEGContainer_5_8, SLOT(onRefreshScales()));
		CENT::connect(this, SIGNAL(dataReceived_5_8(CentData::AnalogData)), 
		EEGContainer_5_8, SIGNAL(dataReceived(CentData::AnalogData)));
		CENT::connect(this, SIGNAL(resetPlots()), EEGContainer_5_8, SLOT(onReset()));
	}

	CENT::connect(timer, SIGNAL(timeout()), EEGContainer, SLOT(onRefreshScales()));
	timer->start();

	NoisePlot* noisePlot = new NoisePlot(tr(NOISE_PLOT_LABEL), this);

	plotsLayout->addWidget(noisePlot);
	layout->addLayout(plotsLayout);

	QList<QWidget*> footerWigets;
	setupButtons(mode, footerWigets);
	if(footerWigets.count() != 0)
	{
		QWidget* footer = new QWidget;
		bool b = UiHelpers::createButtonFooter(*footer, footerWigets, UiHelpers::CenterWidgets);
		Q_ASSERT(b); Q_UNUSED(b);

		layout->addWidget(footer);
	}

	CENT::connect(this, SIGNAL(dataReceived(CentData::AnalogData)), 
		EEGContainer, SIGNAL(dataReceived(CentData::AnalogData)));

	CENT::connect(this,SIGNAL(noiseLevel(const CentData::AnalogData&)),
		noisePlot, SLOT(onDataReceived(const CentData::AnalogData&)));

	CENT::connect(this, SIGNAL(resetPlots()), EEGContainer, SLOT(onReset()));
	CENT::connect(this, SIGNAL(resetPlots()), noisePlot, SLOT(onReset()));
	setLayout(layout);

	m_timer->setInterval(1000); //one second interval
	CENT::connect(m_timer, SIGNAL(timeout()), this, SLOT(onTick()));
}

SetupCapPage::~SetupCapPage()
{
}

void SetupCapPage::activate()
{
	blockSignals(false);
	emit resetPlots();
}

void SetupCapPage::deactivate()
{
	blockSignals(true);
}

QString SetupCapPage::headerText()
{
	return m_title;
}

void SetupCapPage::onDataReceived(const CentData::AnalogData& data)
{
	emit dataReceived(data);
}

void SetupCapPage::onDataReceived_5_8(const CentData::AnalogData& data)
{
	emit dataReceived_5_8(data);
}

void SetupCapPage::onNoiseLevel(const CentData::AnalogData& noise)
{
	emit noiseLevel(noise);
}

void SetupCapPage::startCountdown(int seconds)
{
	if(!m_timeLabel)
	{
		return;
	}
	m_timer->stop();
	m_timeLeft.setHMS(0, 0, 0);
	m_timeLeft = m_timeLeft.addSecs(seconds);
	m_timeLabel->setText(m_timeLeft.toString(TIMER_DISPLAY_FORMAT));
	m_timer->start();
}

void SetupCapPage::onTick()
{
	if(m_timeLeft == QTime(0,0))
	{
		m_timer->stop();
	}
	m_timeLeft = m_timeLeft.addSecs(-1);
	m_timeLabel->setText(m_timeLeft.toString(TIMER_DISPLAY_FORMAT));
}

void SetupCapPage::setState(BaselineState state)
{
	if(m_stateLabel && m_endButton)
	{
		switch(state)
		{
			case BLPrepare:
			{
				m_stateLabel->setText(tr("Prepare..."));
				m_endButton->setEnabled(false);
				break;
			}
			case BLRecording:
			{
				m_stateLabel->setText(tr("Recording"));
				m_endButton->setEnabled(true);
				break;
			}
			case BLPostProcessing:
			{
				m_stateLabel->setText(tr("Processing..."));
				m_endButton->setEnabled(false);
				break;
			}
			default:
			{
				m_stateLabel->setText("");
				m_endButton->setEnabled(false);
			}
		}
	}
}

void SetupCapPage::setupButtons(PageMode mode, QList<QWidget*>& widgets)
{
	switch(mode)
	{
		case SetupCapResearcher:
		{
			QPushButton* skipButton = new QPushButton(tr("Skip Baseline"));
			skipButton->setMaximumWidth(BUTTON_MAX_WIDTH);
			CENT::connect(skipButton, SIGNAL(clicked()), this, SIGNAL(skipBaselineClicked()));

			QPushButton* measureButton = new QPushButton(tr("Measure for:"));
			measureButton->setMaximumWidth(BUTTON_MAX_WIDTH);
			CENT::connect(measureButton, SIGNAL(clicked()), this, SIGNAL(measureBaselineClicked()));

			QSpinBox* spinBox = new QSpinBox();
			spinBox->setRange(BASELINE_TIME_MIN, BASELINE_TIME_MAX);
			spinBox->setMinimumWidth(SPIN_MIN_WIDTH);
			spinBox->setButtonSymbols(QSpinBox::NoButtons);
			CENT::connect(spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(baselineMinutesChanged(int)));

			widgets.append(skipButton);
			widgets.append(measureButton);
			widgets.append(spinBox);
			widgets.append(new QLabel(tr("minutes")));
			break;
		}
		case BaselineResearcher:
		{
			m_stateLabel = new QLabel();
			m_timeLabel = new QLabel();
			m_endButton = new QPushButton(tr("End recording"));
			m_endButton->setMaximumWidth(BUTTON_MAX_WIDTH);
			m_endButton->setEnabled(false);
			CENT::connect(m_endButton, SIGNAL(clicked()), this, SIGNAL(endClicked()));
			
			widgets.append(m_stateLabel);
			widgets.append(m_timeLabel);
			widgets.append(m_endButton);
			break;
		}
		case GameResearcher:
		{
			QPushButton* endButton = new QPushButton(tr("End game"));
			endButton->setMaximumWidth(BUTTON_MAX_WIDTH);
			CENT::connect(endButton, SIGNAL(clicked()), this, SIGNAL(endClicked()));
			
			widgets.append(endButton);
			
			break;
		}
		case SetupCapPatient:
		default:
		{
			break;
		}
	}
}
