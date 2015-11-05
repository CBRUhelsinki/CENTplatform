#ifndef MEDIA_GAME_PLUGIN_H 
#define MEDIA_GAME_PLUGIN_H

#include "ICentGamePlugin.h"
#include "VideoWidgetSurface.h"
#include "Settings.h"
#include "SettingsWidget.h"
#include "AlgorithmManager.h"
#include "ObfuscationAlgorithm.h"
#include "ScoreAlgorithm.h"
#include "MediaGameSummary.h"
#include "MediaGameDataTypes.h"

#include <QPointer>
#include <QMediaPlayer>

class GameWidget;

class MediaGamePlugin: public ICentGamePlugin
{
	Q_OBJECT
	Q_INTERFACES(ICentGamePlugin)
public:
	MediaGamePlugin();
	virtual ~MediaGamePlugin();

public:
	//from ICentGamePlugin
	virtual QString gameName();
	virtual QWidget* gameWidget();
	virtual void releaseWidget();
	virtual bool isConfigurable() const { return true; }

public slots:
	//from ICentGamePlugin
	virtual void onUserInput(CentData::DigitalData data);
	virtual void onExpectedInput(const CentData::DigitalData& data);
	virtual void onEEG(CentData::AnalogData data);
	virtual void onStartGame();
	virtual void onEndGame();
	virtual void onPowerSignal(CentData::AnalogData data);
	virtual void onShowSettings();

private:
	void createConnections();
	void disconnectConnections();
	void runVideo(const QString& fileName);
	void initialize();
	void reset();

private: 
	QPointer<QMediaPlayer>         m_mediaPlayer;
	QPointer<GameWidget>           m_gameWidget;
	QPointer<VideoWidgetSurface>   m_surface;
	QPointer<SettingsWidget>       m_settingsWidget;
	Settings                       m_settings;
	QPointer<ObfuscationAlgorithm> m_obfuscation;
	QPointer<ScoreAlgorithm>       m_score;
	MediaGameSummary               m_mediaGameSummary;
	MediaGameDataType              m_mediaGameDataType;
	AlgorithmManager               m_algorithmManager;
};
#endif //MEDIA_GAME_PLUGIN_H
