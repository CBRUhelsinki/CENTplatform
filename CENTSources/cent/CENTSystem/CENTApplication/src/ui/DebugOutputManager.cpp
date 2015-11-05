#include "DebugOutputManager.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

#include "Connect.h"

DebugOutputManager::DebugOutputManager(QMainWindow* window, OVAppsRunner* runner)
	: QObject(window)
	, m_window(window)
	, m_runner(runner)
{ 
	Q_ASSERT(window);
	if (window)
	{
		// Create debug menu, add there one position and append it to "window" menu bar
		QMenuBar* menu = window->menuBar();
		QMenu* debugMenu = new QMenu(tr("Debug"), menu);
		QAction* consoleAction = debugMenu->addAction(tr("OpenViBE Console Output"));
		CENT::connect(consoleAction, SIGNAL(triggered()), this, SLOT(showConsole()));
		menu->addMenu(debugMenu);
	}
}

DebugOutputManager::~DebugOutputManager()
{
	delete m_console;
}

void DebugOutputManager::showConsole()
{
	if (m_console)
	{
		m_console->show();
		m_console->raise();
	}
	else
	{
		m_console = new ConsoleOutput(m_window);
		m_console->setAttribute(Qt::WA_DeleteOnClose);

		CENT::connect(m_runner.data(), SIGNAL(consoleOutput(QString, QString)), 
		                 m_console.data(), SLOT(print(QString, QString)));
		m_console->show();
	}
}
