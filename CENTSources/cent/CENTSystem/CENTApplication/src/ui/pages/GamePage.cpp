#include "GamePage.h"
#include "Connect.h"
#include "ICentGamePlugin.h"
#include "PowerLevelBar.h"
#include "PowerBarHistoryPlot.h"

#include <QCoreApplication>
#include <QtGlobal>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLayout>

namespace
{
	const int BUTTON_MAX_WIDTH = 100;
}

GamePage::GamePage(QWidget* parent)
	: ICentPage(parent)
	, m_gameWidget(0)
	, m_gamePluginWidget(0)
	, m_userInput(0)
	, m_leftPowerBar(0)
	, m_rightPowerBar(0)
	, m_powerBarHistoryPlot(0)
	, m_gameEnded(true)
{
	createLayout();
}

GamePage::~GamePage()
{

}

void GamePage::activate()
{
	reset();
}

void GamePage::deactivate()
{
}

void GamePage::setGameWidget(QWidget* gamePluginWidget)
{
	if (m_gameWidget && gamePluginWidget != m_gamePluginWidget)
	{
		delete m_gameWidget->layout();
		m_gamePluginWidget = gamePluginWidget;

		if(m_gamePluginWidget)
		{
			QHBoxLayout* gamePluginLayout = new QHBoxLayout;
			gamePluginLayout->addWidget(m_gamePluginWidget, 1);
			m_gameWidget->setLayout(gamePluginLayout);
		}
	}
}

void GamePage::onUserInput(const CentData::DigitalData& data)
{
	if(! m_gameEnded && data.button == CentData::Classification)
	{
		setDebugMsg(QString("User input: <b>%1</b>").arg(data.state));
	}

	emit userInput(data);
	if (m_gamePluginWidget)
	{
		m_gamePluginWidget->update();
	}
}

void GamePage::onPowerSignal(const CentData::AnalogData& data)
{
	m_leftPowerBar->setRange(-1 * data.channel[CentData::POWER_THETA_RANGE], data.channel[CentData::POWER_THETA_RANGE]);
	m_leftPowerBar->setValue(data.channel[CentData::POWER_THETA_LEVEL]);
	m_rightPowerBar->setRange(-1 * data.channel[CentData::POWER_BETA_RANGE], data.channel[CentData::POWER_BETA_RANGE]);
	m_rightPowerBar->setValue(data.channel[CentData::POWER_BETA_LEVEL]);
	m_powerBarHistoryPlot->setPowerRange(-1 * data.channel[CentData::POWER_THETA_RANGE], data.channel[CentData::POWER_THETA_RANGE], 
		                                 -1 * data.channel[CentData::POWER_BETA_RANGE],  data.channel[CentData::POWER_BETA_RANGE]);
	m_powerBarHistoryPlot->setValue(data.channel[CentData::POWER_THETA_LEVEL], data.channel[CentData::POWER_BETA_LEVEL], data.msg_time);

	if (m_gamePluginWidget)
	{
		m_gamePluginWidget->update();
	}
}

void GamePage::onGameEnded()
{
	setDebugMsg(tr("Game ended!"));

	m_gameEnded = true;
}

void GamePage::reset()
{
	m_gameEnded = false;

	m_leftPowerBar->setValue(0.0);
	m_rightPowerBar->setValue(0.0);
	m_powerBarHistoryPlot->reset();

	setDebugMsg(tr("User input: <b>0</b>"));
}

void GamePage::createLayout()
{
	m_gameWidget = new QWidget(this);
	m_userInput = new QLabel();
	m_leftPowerBar = new PowerLevelBar;
	m_rightPowerBar = new PowerLevelBar;
	m_powerBarHistoryPlot = new PowerBarHistoryPlot;

	m_gameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_leftPowerBar->setColors(Qt::green, Qt::yellow);
	m_rightPowerBar->setColors(Qt::yellow, Qt::green);
	m_powerBarHistoryPlot->setColors(Qt::blue, Qt::red);
	QHBoxLayout* centralLayout = new QHBoxLayout;
	centralLayout->addWidget(m_leftPowerBar);
	centralLayout->addWidget(m_gameWidget);
	centralLayout->addWidget(m_rightPowerBar);

	QHBoxLayout* powerBarHistoryLayout = new QHBoxLayout();
	powerBarHistoryLayout->addWidget(m_powerBarHistoryPlot);

	QHBoxLayout* labelLayout = new QHBoxLayout();
	labelLayout->addWidget(m_userInput);
	labelLayout->addStretch();

	//main layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addLayout(centralLayout);
	layout->addLayout(powerBarHistoryLayout);
	layout->addLayout(labelLayout);

	setLayout(layout);
	setFocusPolicy(Qt::StrongFocus);
}

void GamePage::setDebugMsg(const QString& text)
{
#ifndef NDEBUG
	m_userInput->setText(text);
#endif
	Q_UNUSED(text)
}

void GamePage::setGameTimeoutMinutes(int minutes)
{
	m_powerBarHistoryPlot->setTimeAxis(minutes * 60);
}
