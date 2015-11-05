#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>

class Settings : public QObject
{
	Q_OBJECT
public:
	enum VideoMode
	{
		NoVideo,
		VideoImage
	};

public:
	Settings(QObject* parent = 0);
	virtual ~Settings();

	VideoMode getVideoMode() const;
	QString   getFileName() const;

	void setVideoFile(const QString& fileName);
	void setBlankMode(bool mode);

private:
	QSettings* m_userSettings;
};
#endif