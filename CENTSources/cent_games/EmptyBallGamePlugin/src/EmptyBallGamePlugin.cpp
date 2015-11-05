#include "EmptyBallGamePlugin.h"
#include <QtPlugin>
#include <QGLWidget>
#include <QWidget>

#include "EmptyBallGameWidget.h"
#include "Connect.h"
#include <qDebug>

Q_EXPORT_PLUGIN2(EmptyBallGamePlugin, EmptyBallGamePlugin)

namespace
{
	int BUTTON_ON = 1;
}

EmptyBallGamePlugin::EmptyBallGamePlugin()
	: m_expectedHigh(false)
	, m_expectedHighTimestampS(0)
	, m_expectedHighTimestampUs(0)
	, m_userInputHighTimestampS(0)
	, m_userInputHighTimestampUs(0)
	, m_highState(false)

	, m_expectedInput(0)
	, m_userInput(0)
	, m_glWidget(0)
{
	createConnections();
}

void EmptyBallGamePlugin::createConnections()
{
}

EmptyBallGamePlugin::~EmptyBallGamePlugin()
{
	releaseWidget();
}

QString EmptyBallGamePlugin::gameName()
{
	return QString(tr("Empty Ball game - excitement fever"));
}

void EmptyBallGamePlugin::onStartGame()
{
	m_expectedHigh = false;
	m_expectedHighTimestampS = 0;
	m_expectedHighTimestampUs = 0;
	m_userInputHighTimestampS = 0;
	m_userInputHighTimestampUs = 0;
	m_highState = false;
	m_userInput = 0;
	setBallLight();
	emit gameStarted();
}

void EmptyBallGamePlugin::onUserInput(CentData::DigitalData data)
{
	if(data.button == CentData::Classification)
	{
		m_userInput = data.state;
		setBallLight();
	}
}

void EmptyBallGamePlugin::onExpectedInput(const CentData::DigitalData& data)
{
	m_expectedInput = data.state;
	setBallLight();
}

void EmptyBallGamePlugin::setBallLight()
{
	if(! m_glWidget)
	{
		return;
	}

	if (m_expectedInput == m_userInput)
	{
		m_glWidget->setBallLightened(true);
	}
	else
	{
		m_glWidget->setBallLightened(false);
	}
}

void EmptyBallGamePlugin::onEEG(CentData::AnalogData data)
{	//for now ignore
	Q_UNUSED(data);
}

void EmptyBallGamePlugin::onEndGame()
{//we don't store any resources so we don't need to free them, we don't have a summary generator yet.
	emit gameEnded();
}

QWidget* EmptyBallGamePlugin::gameWidget()
{
	if (m_glWidget == NULL)
	{
		m_glWidget = new EmptyBallGameWidget();
	}
	return m_glWidget;
}

void EmptyBallGamePlugin::releaseWidget()
{
	if (m_glWidget != NULL)
	{
		delete m_glWidget;
		m_glWidget = NULL;
	}
}
