#ifndef CENT_GAME_PLUGIN_MANAGER_H
#define CENT_GAME_PLUGIN_MANAGER_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include <QPointer>

class ICentGamePlugin;

class CentGamePluginManager: public QObject
{
	Q_OBJECT
public:
	explicit CentGamePluginManager(QObject* parent = 0);
	virtual ~CentGamePluginManager();

public:
	QMap<QString, bool> availableGames();
	ICentGamePlugin* loadGamePlugin(const QString& gameName);
	QString pluginFileName(const QString& gameName) const;
	int count() const;
	void unloadGamePlugin();

private:
	void readPlugins();
	ICentGamePlugin* getPluginInstance(const QString& filePath);

private:
	struct PluginData
	{
		QString name;
		QString path;
		bool    configurable;
	};
	QMap<QString, PluginData> m_pluginsData;
	QPointer<ICentGamePlugin> m_recentPlugin;
	QPointer<QPluginLoader>   m_pluginLoader;
};

#endif // CENT_GAME_PLUGIN_MANAGER_H

