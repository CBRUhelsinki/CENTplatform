#include "Settings.h"

namespace MediaGameSettings
{
	const QString CENT_ORGANIZATION_NAME = "CENT";
	const QString CENT_APPLICATION_NAME  = "MediaGameSettings";

	const QString VIDEO_PATH             = "VideoPath";
	const QString BLANK_MODE             = "BlankMode";
}

Settings::Settings(QObject* parent)
	: QObject(parent)
{
	m_userSettings = new QSettings(QSettings::UserScope, MediaGameSettings::CENT_ORGANIZATION_NAME, MediaGameSettings::CENT_APPLICATION_NAME);
}

Settings::~Settings()
{
	delete m_userSettings;
}

void Settings::setVideoFile(const QString& fileName)
{
	if(! fileName.isEmpty())
	{
		m_userSettings->setValue(MediaGameSettings::VIDEO_PATH, fileName);
		m_userSettings->setValue(MediaGameSettings::BLANK_MODE, VideoImage);
	}
}

void Settings::setBlankMode(bool mode)
{
	if(mode)
	{
		m_userSettings->setValue(MediaGameSettings::BLANK_MODE, NoVideo);
	}
	else
	{
		m_userSettings->setValue(MediaGameSettings::BLANK_MODE, VideoImage);
	}
}

Settings::VideoMode Settings::getVideoMode() const
{
	return static_cast<VideoMode>(m_userSettings->value(MediaGameSettings::BLANK_MODE).toInt());
}

QString Settings::getFileName() const
{
	return m_userSettings->value(MediaGameSettings::VIDEO_PATH).toString();
}
