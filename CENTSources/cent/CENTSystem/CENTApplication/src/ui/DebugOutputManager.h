#ifndef DEBUG_OUTPUT_MANAGER_H
#define DEBUG_OUTPUT_MANAGER_H

#include <QObject>
#include <QPointer>
#include "OVAppsRunner.h"
#include "ConsoleOutput.h"

class QMainWindow;

/*
 * This is helper class that allows to print a debug information from external in ConsoleOutput window. 
 * Current implementation displays standard and error output from processes executed by OVAppsRunner.
 * When instance of the object is created, it appends "Debug" menu to "window". Then when user clicks
 * "Show console" the manager connects OVAppsRunner with ConsoleOutput so prococesses output is forwarded
 * to ConsoleOutput.
 */
class DebugOutputManager : public QObject
{
	Q_OBJECT
public:
	explicit DebugOutputManager(QMainWindow* window, OVAppsRunner* runner);
	~DebugOutputManager();

private slots:
	void showConsole();

private:
	QMainWindow* m_window;
	QPointer<OVAppsRunner> m_runner;
	QPointer<ConsoleOutput> m_console;
};

#endif
