#ifndef MEDIA_GAME_DATA_TYPES_H
#define MEDIA_GAME_DATA_TYPES_H

#include <QObject>
#include <QList>

class MediaGameDataType : public QObject
{
	Q_OBJECT
public:
	MediaGameDataType(QObject* parent = 0);
	virtual ~MediaGameDataType();

	int           getConstFactor() const;
	int           getTimeoutObfuscation() const;
	QList<int>    getObfuscationLevels() const;
	QList<int>    getAlphaLevels() const;
	QList<double> getWeightArray() const;
	void setObfuscationLevels(const QList<int>& obfuscationLevels);
	void setAlphaLevels(const QList<int>& alphaLevels);
	void setWeightsArray(const QList<double>& weightsArray);

private:
	void loadMediaGameDataType(const QString& filePath);

private:
	int           m_constFactor;
	int           m_timeoutObfuscation;
	QList<int>    m_obfuscationLevels;
	QList<int>    m_alphaLevels;
	QList<double> m_weightsArray;
};

#endif // MEDIA_GAME_DATA_TYPES_H
