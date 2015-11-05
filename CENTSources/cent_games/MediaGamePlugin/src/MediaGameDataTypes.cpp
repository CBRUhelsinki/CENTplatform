#include "MediaGameDataTypes.h"

#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QApplication>

namespace
{
	const QString PLUGINS_DIR                         = "plugins";
	const QString MEDIA_GAME_DATA_TYPE_FILE           = "MediaGamePluginData.ini";

	const QString CONSTANT_FACTOR_SETTING             = "ConstFactor";
	const QString TIMEOUT_OBFUSCATION_TIME_MS_SETTING = "TimeoutObfuscationTimeMs";
	const QString OBFUSCATION_LEVELS_SETTING          = "ObfuscationLevels";
	const QString ALPHA_LEVELS_SETTING                = "AlphaLevels";
	const QString WEIGHTS_ARRAY_SETTING               = "WeightArray";

	const int CONSTANT_FACTOR             = 10;
	const int TIMEOUT_OBFUSCATION_TIME_MS = 2000; //2 second

	const static QStringList OBFUSCATION_LEVELS = QStringList() << "100" << "85" << "70" << "55" << "40" << "25";
	const static QStringList ALPHA_LEVELS       = QStringList() << "0" << "40" << "77" << "115" << "153" << "192";
	const static QStringList WEIGHTS_ARRAY      = QStringList() << "2" << "2" << "1" << "0.1";
};

QList<int> getIntegersList(const QSettings& settings, const QString& key, const QStringList& defaultList)
{
	QStringList tempList = settings.value(key, defaultList).toStringList();
	QList<int> integerList;
	foreach(const QString& value, tempList)
	{
		integerList.append(value.toInt());
	}
	return integerList;
}

QList<double> getDoublesList(const QSettings& settings, const QString& key, const QStringList& defaultList)
{
	QStringList tempList = settings.value(key, defaultList).toStringList();
	QList<double> integerList;
	foreach(const QString& value, tempList)
	{
		integerList.append(value.toDouble());
	}
	return integerList;
}

MediaGameDataType::MediaGameDataType(QObject* parent)
	: QObject(parent)
	, m_constFactor(0)
	, m_timeoutObfuscation(0)
{
	QDir mediaGameDataPath = QApplication::applicationDirPath();
	mediaGameDataPath.cd(PLUGINS_DIR);
	QString mediaGameDataFilePath = mediaGameDataPath.absoluteFilePath(MEDIA_GAME_DATA_TYPE_FILE);
	loadMediaGameDataType(mediaGameDataFilePath);
}

MediaGameDataType::~MediaGameDataType()
{
}

void MediaGameDataType::loadMediaGameDataType(const QString& filePath)
{
	QSettings settings(filePath, QSettings::IniFormat);
	m_constFactor = settings.value(CONSTANT_FACTOR_SETTING, CONSTANT_FACTOR).toInt();
	qDebug() << "Constant Factor = " << m_constFactor;
	m_timeoutObfuscation = settings.value(TIMEOUT_OBFUSCATION_TIME_MS_SETTING, TIMEOUT_OBFUSCATION_TIME_MS).toInt();
	qDebug() << "Timeout Obfuscation settings (Ms) = " << m_timeoutObfuscation;
	m_obfuscationLevels = getIntegersList(settings, OBFUSCATION_LEVELS_SETTING, OBFUSCATION_LEVELS);
	qDebug() << "Obfuscation Levels = " << m_obfuscationLevels;
	m_alphaLevels = getIntegersList(settings, ALPHA_LEVELS_SETTING, ALPHA_LEVELS);
	qDebug() << "Alpha Levels = " << m_alphaLevels;
	m_weightsArray = getDoublesList(settings, WEIGHTS_ARRAY_SETTING, WEIGHTS_ARRAY);
	qDebug() << "Weights Array = " << m_weightsArray;
}

int MediaGameDataType::getConstFactor() const
{
	return m_constFactor;
}

int MediaGameDataType::getTimeoutObfuscation() const
{
	return m_timeoutObfuscation;
}

QList<int> MediaGameDataType::getObfuscationLevels() const
{
	return m_obfuscationLevels;
}

QList<int> MediaGameDataType::getAlphaLevels() const
{
	return m_alphaLevels;
}

QList<double> MediaGameDataType::getWeightArray() const
{
	return m_weightsArray;
}

void MediaGameDataType::setObfuscationLevels(const QList<int>& obfuscationLevels)
{
	m_obfuscationLevels = obfuscationLevels;
}

void MediaGameDataType::setAlphaLevels(const QList<int>& alphaLevels)
{
	m_alphaLevels = alphaLevels;
}

void MediaGameDataType::setWeightsArray(const QList<double>& weightsArray)
{
	m_weightsArray = weightsArray;
}
