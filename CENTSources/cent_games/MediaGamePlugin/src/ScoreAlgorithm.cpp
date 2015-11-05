#include "ScoreAlgorithm.h"
#include "MediaGameDataTypes.h"

#include <QtCore/qmath.h>
#include <QDebug>

namespace
{
	const int CLASS_POSITIVE = 1;
	const int CLASS_NEGATIVE = -1;
}

ScoreAlgorithm::ScoreAlgorithm(MediaGameDataType* mediaGameDataType)
	: m_mediaGameDataType(mediaGameDataType)
	, m_score(0)
	, m_state(CLASS_NEGATIVE)
{
}

ScoreAlgorithm::~ScoreAlgorithm()
{
}

void ScoreAlgorithm::onStart()
{
	m_classifications.clear();
	m_score = 0;
}

void ScoreAlgorithm::onStop()
{
}

void ScoreAlgorithm::onUserInput(CentData::DigitalData data)
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

int ScoreAlgorithm::updateScore()
{
	int score = 0;
	int exponent = 0;
	int scoreIncrement = 0;
	foreach(int state, m_classifications)
	{
		if(state == CLASS_NEGATIVE)
		{
			// negative classification
			exponent = 0;
			score += m_mediaGameDataType->getConstFactor() * scoreIncrement;
			scoreIncrement = 0;
		}
		else
		{
			// positive classification
			exponent++;
			scoreIncrement = qPow(2, exponent);
		}
		qDebug() << "exponent = " << exponent << ", score = "<< score << ", scoreIncrement = " << scoreIncrement;
	}
	score += m_mediaGameDataType->getConstFactor() * scoreIncrement; 
	qDebug() << "exponent = " << exponent << ", score = "<< score << ", scoreIncrement = " << scoreIncrement;
	return score;
}

void ScoreAlgorithm::onExpectedInput(const CentData::DigitalData& data)
{
	Q_UNUSED(data);
}

void ScoreAlgorithm::onTimeout()
{
	QMutexLocker locker( &m_mutex );
	int addScore = updateScore();
	qDebug() << "bonus score = " << addScore;
	emit bonusScore(addScore);
	m_score += addScore;
	qDebug() << "SCORE = " << m_score;
	emit scoreChanged(m_score);
	m_classifications.clear();
}
