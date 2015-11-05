#ifndef CENT_MAIN_WINDOW_CONTROLLER_H
#define CENT_MAIN_WINDOW_CONTROLLER_H

#include <QAction>
#include <QObject>
#include <QPointer>

#include "CentMainWindowControllerBase.h"
#include "StateEnums.h"

class QSettings;
class SettingsWidget;
class AboutWidget;

class CentMainWindowController: public CentMainWindowControllerBase
{
	Q_OBJECT
	Q_ENUMS(CentUiState)

public:
	explicit CentMainWindowController(CentMainWindow& researcherMainWindow, CentMainWindow& patientMainWindow, QObject* parent = 0);
	virtual ~CentMainWindowController();

	QAction* registerMenuItem(const QString& menuName, const QString& menuItemName);

public slots:
	virtual void onUiStateChanged(int state);

private slots:
	void showSettings();
	void showAbout();

private:
	void setupMenu();
	void welcomeState();

private:
	QPointer<QSettings>      m_userSettings;
	QPointer<SettingsWidget> m_settingsWidget;
	QPointer<AboutWidget>    m_aboutWidget;
	QMap<QString, QMenu*>    m_menus;
};

#endif
