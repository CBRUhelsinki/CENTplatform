#include "MediaGamePlugin.h"
#include "GameWidget.h"

#include "Connect.h"

#include <QtPlugin>
#include <qDebug>
#include <QMediaPlaylist>

Q_EXPORT_PLUGIN2(MediaGamePlugin, MediaGamePlugin)

MediaGamePlugin::MediaGamePlugin()
	: m_mediaPlayer(0)
	, m_surface(0)
	, m_gameWidget(0)
	, m_settingsWidget(0)
	, m_obfuscation(0)
	, m_score(0)
	, m_algorithmManager(m_mediaGameDataType)
{
}

MediaGamePlugin::~MediaGamePlugin()
{
	delete m_mediaPlayer;
	delete m_surface;
	delete m_settingsWidget;
	delete m_obfuscation;
	delete m_score;
	releaseWidget();
}

void MediaGamePlugin::initialize()
{
	if(!m_mediaPlayer)
	{
		m_mediaPlayer = new QMediaPlayer(0, QMediaPlayer::VideoSurface);
	}

	if(!m_surface)
	{
		m_surface = new VideoWidgetSurface(this);
		// set video surfface for mediaPlayer
		m_mediaPlayer->setVideoOutput(m_surface);
	}

	if(!m_gameWidget)
	{
		m_gameWidget = new GameWidget;
	}
	// set video surfface for game widget
	m_gameWidget->setVideoSurface(m_surface);
	m_gameWidget->setMediaGameSummary(&m_mediaGameSummary);

	if(!m_obfuscation)
	{
		m_obfuscation = new ObfuscationAlgorithm(&m_mediaGameDataType);
	}
	
	if(!m_score)
	{
		m_score = new ScoreAlgorithm(&m_mediaGameDataType);
	}

	m_algorithmManager.addAlgorithm(m_obfuscation);
	m_algorithmManager.addAlgorithm(m_score);
}

void MediaGamePlugin::reset()
{
	m_mediaGameSummary.reset();
	if(m_mediaPlayer)
	{
		m_mediaPlayer->setPosition(0);
	}
}

void MediaGamePlugin::onStartGame()
{
	reset();
	initialize();
	createConnections();

	m_surface->setFallbackPath(m_settings.getFileName()); // hack for images
	bool blankMode = m_settings.getVideoMode() == Settings::NoVideo;
	m_surface->setBlankMode(blankMode);
	m_algorithmManager.start();

	emit gameStarted();

	if(m_settings.getVideoMode() == Settings::VideoImage)
	{
		runVideo(m_settings.getFileName());
	}
}

void MediaGamePlugin::createConnections()
{
	// set obfuscation and score update slots
	CENT::connect(m_obfuscation, SIGNAL(obfuscationUpdated(int, int)), &m_mediaGameSummary, SLOT(onObfuscationUpdate(int, int)));
	CENT::connect(m_score, SIGNAL(scoreChanged(int)), &m_mediaGameSummary, SLOT(onScoreUpdate(int)));
	CENT::connect(m_score, SIGNAL(bonusScore(int)), &m_mediaGameSummary, SLOT(onBonusScore(int)));
}

void MediaGamePlugin::disconnectConnections()
{
	// disconnect obfuscation and score update slots
	CENT::disconnect(m_obfuscation, SIGNAL(obfuscationUpdated(int, int)), &m_mediaGameSummary, SLOT(onObfuscationUpdate(int, int)));
	CENT::disconnect(m_score, SIGNAL(scoreChanged(int)), &m_mediaGameSummary, SLOT(onScoreUpdate(int)));
	CENT::disconnect(m_score, SIGNAL(bonusScore(int)), &m_mediaGameSummary, SLOT(onBonusScore(int)));
}

QString MediaGamePlugin::gameName()
{
	// Set the name of the game. You will see this name on page in CENT where you can choose a game.
	return tr("Media game - video player");
}

void MediaGamePlugin::onUserInput(CentData::DigitalData data)
{
	qDebug() << "buttonState:" << data.state;
	m_algorithmManager.onUserInput(data);
}

void MediaGamePlugin::onExpectedInput(const CentData::DigitalData& data)
{
	Q_UNUSED(data);
}

void MediaGamePlugin::onEEG(CentData::AnalogData data)
{	//for now ignore
	Q_UNUSED(data);
}

void MediaGamePlugin::onPowerSignal(CentData::AnalogData data)
{	//for now ignore
	Q_UNUSED(data);
}

void MediaGamePlugin::onEndGame()
{//we don't store any resources so we don't need to free them, we don't have a summary generator yet.
	m_mediaPlayer->stop();
	m_surface->stop();
	m_algorithmManager.stop();
	emit gameEnded();
}

QWidget* MediaGamePlugin::gameWidget()
{
	if(!m_gameWidget)
	{
		m_gameWidget = new GameWidget;
	}
	return m_gameWidget;
}

void MediaGamePlugin::releaseWidget()
{
	if(m_gameWidget)
	{
		delete m_gameWidget;
		m_gameWidget = NULL;
	}
}

void MediaGamePlugin::onShowSettings()
{
	if(!m_settingsWidget)
	{
		m_settingsWidget = new SettingsWidget(&m_settings);
		m_settingsWidget->setObjectName("CentSettingsWidget");
	}
	m_settingsWidget->show();
	m_settingsWidget->raise();
}

void MediaGamePlugin::runVideo(const QString& fileName)
{
	if(!fileName.isEmpty())
	{
		QUrl url = QUrl::fromLocalFile(fileName);

		QMediaPlaylist * playlist = new QMediaPlaylist();

		playlist->addMedia(url);
		playlist->setPlaybackMode( QMediaPlaylist::Loop );

		m_mediaPlayer->setPlaylist( playlist );
		playlist->setCurrentIndex(0);
		m_mediaPlayer->play();
	}
}
