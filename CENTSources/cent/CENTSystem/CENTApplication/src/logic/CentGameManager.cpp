#include "CentGameManager.h"
#include "CentGamePluginManager.h"

CentGameManager::CentGameManager(QObject* parent)
	: QObject(parent)
{
}

CentGameManager::~CentGameManager()
{
}

bool CentGameManager::changeCurrentGame(QString gameName)
{
	if(m_currentGamePluginPointer)
	{
		if(gameName == m_currentGamePluginPointer->gameName())
		{
			// This game was selected so it is still selected
			return true;
		}
		emit gamePluginUnloading();
		m_gamePluginManager.unloadGamePlugin();
		m_currentGamePluginPointer = NULL;
	}
	ICentGamePlugin* gamePlugin = m_gamePluginManager.loadGamePlugin(gameName);
	QString fileName = m_gamePluginManager.pluginFileName(gameName);

	if(gamePlugin)
	{
		m_currentGamePluginPointer = gamePlugin;
		emit gamePluginChanged(m_currentGamePluginPointer, fileName);
		return true;
	}

	return false;
}

bool CentGameManager::showSettings(QString gameName)
{
	if(m_currentGamePluginPointer)
	{
		if(gameName == m_currentGamePluginPointer->gameName())
		{
			m_currentGamePluginPointer->onShowSettings();
			return true;
		}
	}
	if(!changeCurrentGame(gameName))
	{
		return false;
	}

	m_currentGamePluginPointer->onShowSettings();

	return true;
}

void CentGameManager::resetSession()
{
	if(m_currentGamePluginPointer)
	{
		delete m_currentGamePluginPointer;
	}
}

QMap<QString, bool> CentGameManager::availableGames()
{
	return m_gamePluginManager.availableGames();
}
