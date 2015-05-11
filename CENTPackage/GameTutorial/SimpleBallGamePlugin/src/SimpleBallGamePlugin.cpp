#include "SimpleBallGamePlugin.h"

#include "Connect.h"
#include "SimpleBallGameLogic.h"
#include "SimpleBallGameSummary.h"
#include "SimpleBallGameWidget.h"

#include <QtPlugin>
#include <qDebug>

Q_EXPORT_PLUGIN2(SimpleBallGamePlugin, SimpleBallGamePlugin)

SimpleBallGamePlugin::SimpleBallGamePlugin()
	: m_logic(0)
	, m_summary(0)
	, m_glWidget(0)
{
}

SimpleBallGamePlugin::~SimpleBallGamePlugin()
{
	if(m_logic != NULL)
	{
		delete m_logic;
	}
	releaseWidget();
}

QString SimpleBallGamePlugin::gameName()
{
	// Set the name of the game. You will see this name on page in CENT where you can choose a game.
	return tr("Simple Ball game - excitement fever");
}

void SimpleBallGamePlugin::onUserInput(CentData::DigitalData data)
{
	qDebug() << "button:" << data.button;
	qDebug() << "buttonState:" << data.state;
	m_logic->onUserInput(data);
}

void SimpleBallGamePlugin::onExpectedInput(const CentData::DigitalData& data)
{
	m_logic->onExpectedInput(data);
}

void SimpleBallGamePlugin::onEEG(CentData::AnalogData data)
{	//for now ignore
	Q_UNUSED(data);
}

void SimpleBallGamePlugin::onPowerSignal(CentData::AnalogData data)
{	//for now ignore
	Q_UNUSED(data);
}

void SimpleBallGamePlugin::onStartGame()
{
	//TODO: this is copied from previous reset(), still needs refactoring
	disconnectConnections();

	delete m_logic;
	delete m_summary;

	m_logic = new SimpleBallGameLogic(this);
	m_summary = new SimpleBallGameSummary(this);

	createConnections();
	m_logic->startGame();

	emit gameStarted();
}

void SimpleBallGamePlugin::onEndGame()
{//we don't store any resources so we don't need to free them, we don't have a summary generator yet.
	emit gameEnded();
}

void SimpleBallGamePlugin::createConnections()
{
	if(m_glWidget && m_logic)
	{
		CENT::connect(m_logic, SIGNAL(redrawGame()), m_glWidget, SLOT(repaint()));
		CENT::connect(m_logic, SIGNAL(ballMoved(const QPointF&)), m_glWidget, SLOT(setBallPosition(const QPointF&)));
	}
}

void SimpleBallGamePlugin::disconnectConnections()
{
	if(m_glWidget && m_logic)
	{
		CENT::disconnect(m_logic, SIGNAL(redrawGame()), m_glWidget, SLOT(repaint()));
		CENT::disconnect(m_logic, SIGNAL(ballMoved(const QPointF&)), m_glWidget, SLOT(setBallPosition(const QPointF&)));
	}
}

QWidget* SimpleBallGamePlugin::gameWidget()
{
	if(m_glWidget == NULL)
	{
		m_glWidget = new SimpleBallGameWidget();
	}
	return m_glWidget;
}

void SimpleBallGamePlugin::releaseWidget()
{
	if(m_glWidget != NULL)
	{
		delete m_glWidget;
		m_glWidget = NULL;
	}
}
