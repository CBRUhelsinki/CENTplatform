#include "BaselineInfoPage.h"

#include <QCheckBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <cmath>
#include "BaseLinePlot.h"
#include "connect.h"
#include "PowerSpectrumData.h"
#include "UiHelpers.h"

namespace
{
	const double MAX_FREQUENCY = 100.0;
	const int CHANNEL = 0;

	const double DEFAULT_COEF_BETA = 1.0;
	const double DEFAULT_COEF_THETA = 1.0;
}

BaselineInfoPage::BaselineInfoPage(QWidget* parent)
	: ICentPage(parent)
	, m_betaLevelLabel(0)
	, m_thetaLevelLabel(0)
	, m_spinCoefBeta(0)
	, m_spinCoefTheta(0)
	, m_reRecordButton(new QPushButton(tr("Re-record")))
	, m_nextButton(new QPushButton(tr("Next")))
	, m_plot(new BaseLinePlot(tr("Baseline session results")))
	, m_x(0)
	, m_y(0)
	, m_size(0)
{
	createLayout();
	CENT::connect(m_reRecordButton, SIGNAL(clicked()), this, SIGNAL(reRecord()));
	CENT::connect(m_nextButton, SIGNAL(clicked()), this, SIGNAL(next()));
	resetSession();
}

BaselineInfoPage::~BaselineInfoPage()
{
}

void BaselineInfoPage::activate()
{
	m_plot->setData(m_x, m_y, m_size);
	onGameTimeChange();
}

void BaselineInfoPage::resetSession()
{
	//reset coeffs
	m_spinCoefBeta->setValue(DEFAULT_COEF_BETA);
	m_spinCoefTheta->setValue(DEFAULT_COEF_THETA);
}

void BaselineInfoPage::deactivate()
{
	delete [] m_x;
	m_x = 0;
	delete [] m_y;
	m_y = 0;
	m_size = 0;
}

void BaselineInfoPage::setDisplayData(const PowerSpectrumData& data)
{
	const int channelCount = data.channelCount();
	if (channelCount > 0)
	{
		const int records = data.recordCount();
		if (records > 0)
		{
			preparePlot(CHANNEL, records, data);
		}
		else
		{
			qDebug() << "Number of rows is not valid: " << records << "!";
		}
	}
	else
	{
		qDebug() << "Data does not contain any needed information!";
	}
}

void BaselineInfoPage::setBetaThetaLevels(double beta, double theta)
{
	if(m_betaLevelLabel && m_thetaLevelLabel)
	{
		m_betaLevelLabel->setText(QString::number(beta));
		m_thetaLevelLabel->setText(QString::number(theta));
	}
}

double BaselineInfoPage::betaCoeffValue()
{
	return m_spinCoefBeta->value();
}

double BaselineInfoPage::thetaCoeffValue()
{
	return m_spinCoefTheta->value();
}

void BaselineInfoPage::onGameTimeChange()
{
	m_spinGameDuration->setEnabled(m_gameTimeoutCheckBox->checkState() == Qt::Checked);
	int minutes = m_gameTimeoutCheckBox->checkState() == Qt::Checked ? m_spinGameDuration->value() : -1;
	emit gameTimeoutChanged(minutes);
}

void BaselineInfoPage::createLayout()
{
	QHBoxLayout* horizontalLayout = new QHBoxLayout();

	QLabel* labelBeta = new QLabel(tr("Beta:"));
	QLabel* labelTheta = new QLabel(tr("Theta"));
	QLabel* labelCoefB = new QLabel(tr("Beta coeff:"));
	QLabel* labelCoefT = new QLabel(tr("Theta coeff:"));
	QLabel* minutesLabel = new QLabel(tr("minutes."));

	m_betaLevelLabel = new QLabel("0");
	m_betaLevelLabel->setObjectName("BetaLevelLabel");
	m_thetaLevelLabel = new QLabel("0");
	m_thetaLevelLabel->setObjectName("ThetaLevelLabel");

	m_spinCoefBeta = new QDoubleSpinBox();
	m_spinCoefTheta = new QDoubleSpinBox();

	m_spinCoefBeta->setValue(DEFAULT_COEF_BETA);
	m_spinCoefTheta->setValue(DEFAULT_COEF_THETA);

	m_spinCoefBeta->setDecimals(2);
	m_spinCoefTheta->setDecimals(2);

	m_spinCoefBeta->setSingleStep(0.01);
	m_spinCoefTheta->setSingleStep(0.01);

	m_spinCoefBeta->setButtonSymbols(QSpinBox::NoButtons);
	m_spinCoefTheta->setButtonSymbols(QSpinBox::NoButtons);

	// Game duration widgets:
	m_gameTimeoutCheckBox = new QCheckBox(tr("Limit game to:"));
	m_spinGameDuration = new QSpinBox;
	m_spinGameDuration->setButtonSymbols(QSpinBox::NoButtons);
	m_spinGameDuration->setEnabled(false);
	m_spinGameDuration->setRange(1, 60);

	CENT::connect(m_gameTimeoutCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onGameTimeChange()));
	CENT::connect(m_spinGameDuration, SIGNAL(valueChanged(int)), this, SLOT(onGameTimeChange()));

	// Construct layout
	horizontalLayout->addStretch();
	horizontalLayout->addWidget(labelBeta);
	horizontalLayout->addWidget(m_betaLevelLabel);

	horizontalLayout->addStretch();
	horizontalLayout->addWidget(labelTheta);
	horizontalLayout->addWidget(m_thetaLevelLabel);

	horizontalLayout->addStretch();
	horizontalLayout->addWidget(labelCoefB);
	horizontalLayout->addWidget(m_spinCoefBeta);

	horizontalLayout->addStretch();
	horizontalLayout->addWidget(labelCoefT);
	horizontalLayout->addWidget(m_spinCoefTheta);

	horizontalLayout->addStretch();
	horizontalLayout->addWidget(m_gameTimeoutCheckBox);
	horizontalLayout->addWidget(m_spinGameDuration);
	horizontalLayout->addWidget(minutesLabel);
	horizontalLayout->addStretch();

	QWidget* buttonWidget = new QWidget();
	QList<QWidget*> btnList;
	btnList << m_reRecordButton << m_nextButton;

	bool b = UiHelpers::createButtonFooter(*buttonWidget, btnList, UiHelpers::AllRight);
	Q_ASSERT(b); Q_UNUSED(b);

	// Main layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(m_plot);
	layout->addLayout(horizontalLayout);
	layout->addWidget(buttonWidget);

	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

void BaselineInfoPage::preparePlot(int channel, int sampleCount, const PowerSpectrumData& data)
{
	if (0 < sampleCount)
	{
		m_size = sampleCount;
		m_x = new double[m_size];
		m_y = new double[m_size];

		const int epochs = data.epochCount();
		PowerSpectrumRecord record;
		for (int i = 0; i < m_size; ++i)
		{
			record = data.record(channel, i);
			m_x[i] = record.m_maxFrequency + (record.m_maxFrequency - record.m_minFrequency)/2;
			m_y[i] = log10f(record.m_value / epochs);
		}
	}
}
