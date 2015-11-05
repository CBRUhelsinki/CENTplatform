#include "MediaGameSummary.h"
#include "Connect.h"

#include <QTimer>

namespace
{
	const int TIMEOUT       = 10;
	const int BONUS_TIMEOUT = 1000;
	const int FULL_FRAME    = 100;
	const int ALPHA_SHIFT   = 2;
}

MediaGameSummary::MediaGameSummary(QObject *parent)
	: QObject(parent)
	, m_score(0)
	, m_bonusScore(0)
	, m_alphaLevel(0)
	, m_currentClippingFrame(FULL_FRAME)
	, m_setClippingFrame(0)
	, m_setAlphaLevel(0)
{
	m_tickTimer = new QTimer(this);
	m_tickTimer->setInterval(TIMEOUT);

	CENT::connect(m_tickTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

	m_bonusScoreTimer = new QTimer(this);
	m_bonusScoreTimer->setInterval(BONUS_TIMEOUT);

	CENT::connect(m_bonusScoreTimer, SIGNAL(timeout()), this, SLOT(onBonusScoreTimeout()));
}

MediaGameSummary::~MediaGameSummary()
{
}

void MediaGameSummary::reset()
{
	m_score = 0;
	m_bonusScore = 0;
	m_alphaLevel = 0;
	m_currentClippingFrame = FULL_FRAME;
	m_setClippingFrame = 0;
	m_setAlphaLevel = 0;
}

void MediaGameSummary::onScoreUpdate(int score)
{
	m_score = score;
	if(score > 0)
	{
		m_bonusScoreTimer->start();
	}
}

void MediaGameSummary::onBonusScore(int score)
{
	m_bonusScore = score;
}

void MediaGameSummary::onBonusScoreTimeout()
{
	m_bonusScoreTimer->stop();
	m_bonusScore = 0;
}

void MediaGameSummary::onTimeout()
{
	if(m_setClippingFrame == m_currentClippingFrame && m_setAlphaLevel == m_alphaLevel)
	{
		m_tickTimer->stop();
		return;
	}
	updateClippingFrame();
	
	updateAlphaLevel();
}

void MediaGameSummary::onObfuscationUpdate(int alphaLevel, int clippingFrame)
{
	// TODO good and fast algorith for alpha and obfuscation update
	m_setAlphaLevel    = alphaLevel;
	m_setClippingFrame = clippingFrame;
	m_tickTimer->start();
}

int MediaGameSummary::getScore()
{
	return m_score;
}

int MediaGameSummary::getAlphaLevel()
{
	return m_alphaLevel;
}

int MediaGameSummary::getClippingFrame()
{
	return m_currentClippingFrame;
}

int MediaGameSummary::getBonusscore()
{
	return m_bonusScore;
}

void MediaGameSummary::updateClippingFrame()
{
	if(m_setClippingFrame != m_currentClippingFrame)
	{
		if(m_setClippingFrame > m_currentClippingFrame)
		{
			m_currentClippingFrame++;
		}
		else
		{
			m_currentClippingFrame--;
		}
	}
}

void MediaGameSummary::updateAlphaLevel()
{
	if(m_setAlphaLevel != m_alphaLevel)
	{
		if(m_setAlphaLevel > m_alphaLevel)
		{
			m_alphaLevel += ALPHA_SHIFT;
		}
		else
		{
			m_alphaLevel -= ALPHA_SHIFT;
		}
	}
}
