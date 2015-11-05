#include "CentGamePluginManager.h"
#include "ICentGamePlugin.h"
#include "Connect.h"

#include <QApplication>
#include <QFileInfo>
#include <QDir>

namespace
{
	const QString PLUGINS_DIR = "plugins";
	const QString PLUGINS_EXT = "*.dll";
}

CentGamePluginManager::CentGamePluginManager(QObject* parent)
	: QObject(parent)
{
	readPlugins();
}

CentGamePluginManager::~CentGamePluginManager()
{
	unloadGamePlugin();
}

QMap<QString, bool> CentGamePluginManager::availableGames()
{
	QMap<QString, bool> availableGames;

	foreach (const PluginData& pluginData, m_pluginsData)
	{
		availableGames.insert(pluginData.name, pluginData.configurable);
	}
	return availableGames;
}

ICentGamePlugin* CentGamePluginManager::loadGamePlugin(const QString& gameName)
{
	QString filePath = m_pluginsData.value(gameName).path;
	if (filePath.isEmpty())
	{
		return NULL;
	}
	m_recentPlugin = getPluginInstance(filePath);
	return m_recentPlugin;
}

QString CentGamePluginManager::pluginFileName(const QString& gameName) const
{
	QString filePath = m_pluginsData.value(gameName).path;
	QFileInfo pathInfo( filePath );
	return pathInfo.baseName();
}

int CentGamePluginManager::count() const
{
	return m_pluginsData.size();
}

void CentGamePluginManager::readPlugins()
{
	QDir pluginsDir = QApplication::applicationDirPath();
	pluginsDir.cd(PLUGINS_DIR);
	QString absoluteFilePath;
	QStringList filters(PLUGINS_EXT);

	foreach(QString fileName, pluginsDir.entryList(filters, QDir::Files))
	{
		absoluteFilePath = pluginsDir.absoluteFilePath(fileName);

		QPluginLoader loader(absoluteFilePath);
		QObject* instance = loader.instance();
		ICentGamePlugin* gamePlugin = qobject_cast<ICentGamePlugin*>(instance);
		if (gamePlugin)
		{
			PluginData pluginData;
			pluginData.name         = gamePlugin->gameName();
			pluginData.path         = absoluteFilePath;
			pluginData.configurable = gamePlugin->isConfigurable();
			m_pluginsData.insert(gamePlugin->gameName(), pluginData);
			delete gamePlugin;
		}
		loader.unload();
	}
}

ICentGamePlugin* CentGamePluginManager::getPluginInstance(const QString& filePath)
{
	unloadGamePlugin();

	m_pluginLoader = new QPluginLoader(filePath);
	QObject* instance = m_pluginLoader->instance();
	if (instance)
	{
		ICentGamePlugin* gamePlugin = qobject_cast<ICentGamePlugin*>(instance);
		if (gamePlugin)
		{
			return gamePlugin;
		}
		delete instance;
	}
	m_pluginLoader->unload();
	delete m_pluginLoader;
	return NULL;
}

void CentGamePluginManager::unloadGamePlugin()
{
	if (m_recentPlugin)
	{
		Q_ASSERT(m_pluginLoader);
		delete m_recentPlugin;
	}
	if (m_pluginLoader && m_pluginLoader->isLoaded())
	{
		m_pluginLoader->unload();
		delete m_pluginLoader;
		m_pluginLoader = NULL;
	}
}
