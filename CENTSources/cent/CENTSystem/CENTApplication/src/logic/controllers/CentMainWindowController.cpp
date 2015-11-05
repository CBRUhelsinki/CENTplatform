#include "CentMainWindowController.h"

#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSettings>

#include "AboutWidget.h"
#include "CentConstants.h"
#include "CentMainWindow.h"
#include "Connect.h"
#include "MainWindowCommon.h"
#include "SettingsWidget.h"

namespace
{
	const QString ABOUT_TXT_PATH = ":/texts/about.txt";
}

CentMainWindowController::CentMainWindowController(CentMainWindow& researcherMainWindow, CentMainWindow& patientMainWindow, QObject* parent)
	: CentMainWindowControllerBase(researcherMainWindow, patientMainWindow, parent)
{
	m_userSettings = new QSettings(QSettings::UserScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);
	setupMenu();
	createConnections();
}

CentMainWindowController::~CentMainWindowController()
{
	delete m_aboutWidget;
	delete m_settingsWidget;
	delete m_userSettings;
}

void CentMainWindowController::onUiStateChanged(int state)
{
	switch (state)
	{
		case UiWelcomeState:
			welcomeState();
			break;

		case UiNotInitialized:
		case UiPatientQuestionaryState:
		case CentChooseScenarioState:
		case UiCapSetupState:
		case UiBaseLineState:
		case UiBaseLineInfoState:
		case UiGameSelectState:
		case UiTutorialState:
		case UiGameState:
		case UiSummaryState:
		default:
			break;
	}
}

void CentMainWindowController::showSettings()
{
	if(!m_settingsWidget)
	{
		m_settingsWidget = new SettingsWidget(*m_userSettings);
		m_settingsWidget->setObjectName("CentSettingsWidget");
	}
	m_settingsWidget->show();
	m_settingsWidget->raise();
}

void CentMainWindowController::showAbout()
{
	if(!m_aboutWidget)
	{
		QFile aboutFile(ABOUT_TXT_PATH);
		if(!aboutFile.open(QIODevice::ReadOnly))
		{
			qFatal("failed to open about.txt from resources");
		}
		QString aboutText = aboutFile.readAll();
		aboutFile.close();
		m_aboutWidget = new AboutWidget(aboutText);
		m_aboutWidget->setObjectName("CentAboutWidget");
	}
	m_aboutWidget->show();
	m_aboutWidget->raise();
}

void CentMainWindowController::welcomeState()
{
	m_researcherMainWindow.show();
	m_patientMainWindow.show();
}

void CentMainWindowController::setupMenu()
{
	//TODO: all menus should be created here for consistency
	QMenuBar* menuBar = m_researcherMainWindow.menuBar();
	QMenu* toolsMenu = new QMenu(tr(CENT::Menu::TOOLS_MENU_NAME), menuBar);
	QAction* settingsAction = toolsMenu->addAction(tr("Settings"));
	CENT::connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));
	menuBar->addMenu(toolsMenu);
	m_menus.insert(QString(CENT::Menu::TOOLS_MENU_NAME), toolsMenu);

	QMenu* helpMenu = new QMenu(tr(CENT::Menu::HELP_MENU_NAME), menuBar);
	QAction* aboutAction = helpMenu->addAction(tr("About"));

	CENT::connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
	menuBar->addMenu(helpMenu);
	m_menus.insert(QString(CENT::Menu::HELP_MENU_NAME), helpMenu);
}

QAction* CentMainWindowController::registerMenuItem(const QString& menuName, const QString& menuItemName)
{
	QMenu* menu = m_menus.value(menuName, 0);
	if(!menu)
	{
		return 0;
	}
	QAction* action = 0;
	foreach(QAction* ac, menu->actions())
	{
		if(ac->text() == menuItemName)
		{
			action = ac;
		}
	}
	if(!action)
	{
		action = menu->addAction(menuItemName);
	}
	return action;
}
