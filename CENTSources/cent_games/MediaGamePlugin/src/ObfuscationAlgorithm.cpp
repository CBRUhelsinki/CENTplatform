#include "ObfuscationAlgorithm.h"

#include <QMutexLocker>
#include <QDebug>

namespace
{
	const int CLASS_POSITIVE = 1;
	const int CLASS_NEGATIVE = -1;
}

ObfuscationAlgorithm::ObfuscationAlgorithm(MediaGameDataType *mediaGameData)
	: m_mediaGameDataType(mediaGameData)
	, m_currentObfuscation(0)
	, m_alphaLevel(0)
	, m_clippingFrame(0)
	, m_state(CLASS_NEGATIVE)
{
}

ObfuscationAlgorithm::~ObfuscationAlgorithm()
{
}

void ObfuscationAlgorithm::onStart()
{
	reset();
}

void ObfuscationAlgorithm::reset()
{
	m_classifications.clear();
	m_alphaLevel = 0;
	m_clippingFrame = 0;
}

void ObfuscationAlgorithm::onStop()
{
}

int ObfuscationAlgorithm::updateObfuscationLevel()
{
	int startIndex = 0;
	int maxIndex = m_classifications.length();
	qDebug() << "m_classifications.length() = " << maxIndex;
	int weightArrayLength = m_mediaGameDataType->getWeightArray().length();
	if(weightArrayLength < m_classifications.length())
	{
		startIndex = m_classifications.length() - weightArrayLength - 1;
		maxIndex = weightArrayLength;
	}
	float dL = 0.0L;
	for(int i = 0; i < maxIndex; i++)
	{
		dL += m_classifications[startIndex + i] * m_mediaGameDataType->getWeightArray()[i];
		qDebug() << "dL(" << i << ") =" << dL;
	}
	qDebug() << "before update m_currentObfuscation = " << m_currentObfuscation;
	m_currentObfuscation = m_currentObfuscation - dL;

	// normalize m_currentObfuscation not to exceed 
	m_currentObfuscation = qBound<float>(0.0, m_currentObfuscation, static_cast<float>(m_mediaGameDataType->getObfuscationLevels().count()));

	qDebug() << "finall m_currentObfuscation = " << m_currentObfuscation;

	return qRound(m_currentObfuscation);
}

void ObfuscationAlgorithm::onUserInput(CentData::DigitalData data)
{
	QMutexLocker locker( &m_mutex );
	switch(data.button)
	{
		case CentData::Classification:
		{
			int state = data.state ? CLASS_POSITIVE : CLASS_NEGATIVE;
			m_state = state;
			break;
		}
		case CentData::EpochNotifier:
		{
			m_classifications.append(m_state);
			break;
		}
		default:
		{
			qWarning() << __FUNCTION__ << "Unsupported digital signal from OpenVibe";
			break;
		}
	}
}

void ObfuscationAlgorithm::onExpectedInput(const CentData::DigitalData& data)
{
	Q_UNUSED(data); // Not supported by Media game
}

int ObfuscationAlgorithm::updateAlphaLevel(int obfuscationLevel)
{
	if(obfuscationLevel < 0)
	{
		return m_mediaGameDataType->getAlphaLevels().first();
	}
	if(obfuscationLevel > (m_mediaGameDataType->getAlphaLevels().size() - 1))
	{
		return m_mediaGameDataType->getAlphaLevels().last();
	}
	return m_mediaGameDataType->getAlphaLevels()[obfuscationLevel];
}

int ObfuscationAlgorithm::updateClippingFrameLevel(int obfuscationLevel)
{
	if(obfuscationLevel < 0)
	{
		return m_mediaGameDataType->getObfuscationLevels().first();
	}
	if(obfuscationLevel > (m_mediaGameDataType->getObfuscationLevels().size() - 1))
	{
		return m_mediaGameDataType->getObfuscationLevels().last();
	}
	return m_mediaGameDataType->getObfuscationLevels()[obfuscationLevel];
}

void ObfuscationAlgorithm::onTimeout()
{
	QMutexLocker locker( &m_mutex );
	int obfuscationLevel = updateObfuscationLevel();
	m_alphaLevel = updateAlphaLevel(obfuscationLevel);
	m_clippingFrame = updateClippingFrameLevel(obfuscationLevel);
	emit obfuscationUpdated(m_alphaLevel, m_clippingFrame);
	reset();
}
