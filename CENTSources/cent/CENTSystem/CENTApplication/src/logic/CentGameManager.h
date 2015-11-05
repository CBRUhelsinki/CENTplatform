#ifndef CENT_GAME_MANAGER_H
#define CENT_GAME_MANAGER_H

#include <QObject>
#include <QStringList>
#include <QPointer>
#include "CentGamePluginManager.h"
#include "ICentGamePlugin.h"

class CentGameManager: public QObject
{
	Q_OBJECT
public:
	explicit CentGameManager(QObject* parent = 0);
	virtual ~CentGameManager();
	bool changeCurrentGame(QString gameName);
	bool showSettings(QString gameName);

	QMap<QString, bool> availableGames();

public slots:
	void resetSession();

signals:
	void gamePluginUnloading();
	void gamePluginChanged(ICentGamePlugin* gamePlugin, QString fileName);

private:
	CentGamePluginManager m_gamePluginManager;
	QPointer<ICentGamePlugin> m_currentGamePluginPointer;
};

#endif // CENT_GAME_MANAGER_H
