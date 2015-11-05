#include "SimpleBallGameLogic.h"

#include <QMutex>
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <QDebug>
#include <QThread>

#include "Connect.h"

namespace
{
	const int BUTTON_ON = 1;
	const int BUTTON_OFF = 0;
	const int BUTTON_Simple_BALL_GAME = 0;
	const int BUTTON_Simple_BALL_GAME_EXPECTED = 3538136;

	const int MAX_HEIGHT = 100;
	const int MIN_HEIGHT = 0;

	const char SCORE_PLUS_FULL = 2;
	const char SCORE_PLUS_NORMAL = 1;
	const char SCORE_MINUS = -1;

	const int TIMEOUT_REDRAW_TIMER_MS = 20;
	const int TIMEOUT_GAME_TIME_MS = 1000;	//1 second
	const float BALL_START_Y = 50;
	
	const float START_BALL_MASS = 1.0f; //in kg
	const float G_FORCE = 80.0f;
	const float BALL_FORCE = START_BALL_MASS*G_FORCE + 200.0f;
}


class MutexHolder
{
public:
	explicit MutexHolder(QMutex* mutex)
		: m_mutex(mutex)
	{
		while (! m_mutex->tryLock())
		{
			qApp->processEvents(QEventLoop::AllEvents,5);
		}
	}
	virtual ~MutexHolder()
	{
		m_mutex->unlock();
	}
	QMutex* m_mutex;
};

SimpleBallGameLogic::SimpleBallGameLogic(QObject* parent)
	: QObject(parent) 
	, m_gameOver(false)
	, m_highState(false)
	, m_expectedHigh(true)
	, m_rising(false)
	, m_ballM(START_BALL_MASS)
	, m_lastS(0)
	, m_lastUs(0)
	, m_overalTimePlus(0.0f)
	, m_overalTimeMinus(0.0f)
	, m_ballF(BALL_FORCE)
	, m_gForce(G_FORCE)
	, m_firstInput(true)
	, m_ballV(0.0)
{
	m_ballPoint.rx() = 0;
	m_ballPoint.ry() = BALL_START_Y;

	m_redrawTimer = new QTimer(this);
	m_redrawTimer->setInterval(TIMEOUT_REDRAW_TIMER_MS);
	
	CENT::connect(m_redrawTimer, SIGNAL(timeout()), this, SLOT(onRedraw()));

	m_gameTimer = new QTimer(this);
	m_gameTimer->setInterval(TIMEOUT_GAME_TIME_MS);

	m_ballA = ((m_ballF - m_ballM * m_gForce) / m_ballM); 
	m_ballANegative = m_gForce;

	CENT::connect(m_gameTimer, SIGNAL(timeout()), this, SLOT(onGameEnd()));
	m_fpsTimer.start();
}

SimpleBallGameLogic::~SimpleBallGameLogic()
{
	m_redrawTimer->stop();
	m_gameTimer->stop();
}

void SimpleBallGameLogic::onGameEnd()
{
	MutexHolder locker(&m_gameOverMutex);
	m_gameOver = true;
}

void SimpleBallGameLogic::startGame()
{
	m_redrawTimer->start();
	m_ballPoint.rx() = 0;
	m_ballPoint.ry() = BALL_START_Y;
	emit ballMoved(m_ballPoint);
}

const QPointF& SimpleBallGameLogic::ballPoint()
{
	MutexHolder lock(&m_ballPointMutex);
	return m_ballPoint;
}

const bool SimpleBallGameLogic::isRising()
{
	MutexHolder lock(&m_risingMutex);
	return m_rising;
}

const bool SimpleBallGameLogic::isGameOver()
{
	MutexHolder lock(&m_gameOverMutex);
	return m_gameOver;
}

float SimpleBallGameLogic::countPath(const float& time, const float& a)
{
	return (a * time * time)/2.0f ;  //1m = 1px;
}

void SimpleBallGameLogic::onUserInput(CentData::DigitalData data)
{
	if(data.button != CentData::Classification)
	{
		return;
	}
	MutexHolder locker(&m_timestampMutex);
	qDebug()<< "timestamp: " << data.msg_time.tv_sec << "," << data.msg_time.tv_usec;

	bool willBeHigh = ((data.button == BUTTON_Simple_BALL_GAME) && (data.state == BUTTON_ON)) && m_expectedHigh;

	bool broken = !(willBeHigh && m_highState || !willBeHigh && !m_highState);
	m_highState = willBeHigh;
	
	if(! m_firstInput)
	{
		if(m_highState && broken)
		{//changing state from low to high
			//m_overalTimeMinus += data.msg_time.tv_sec - m_lastS + (data.msg_time.tv_usec - m_lastUs) /1000000;
			qDebug() << "changing state from low to high";
		}
		else if ( m_highState && ! broken)
		{//high state stays for a while, do nothing, probably an error
			qWarning() << "continue of high state, OpenVibeError";
		}
		else if (! m_highState && ! broken)
		{//it's a low state for a while, do nothing, probably an error
			qWarning() << "continue of low state, OpenVibeError";
		}
		else if (! m_highState && broken)
		{//it's a walk from high state to lower state
			m_overalTimePlus += data.msg_time.tv_sec - m_lastS + (data.msg_time.tv_usec - m_lastUs) /1000000;
		}
	}
	m_lastS = data.msg_time.tv_sec;
	m_lastUs = data.msg_time.tv_usec;

	m_firstInput = false;
}

void SimpleBallGameLogic::onExpectedInput(const CentData::DigitalData& data)
{
	Q_UNUSED(data);
}

long SimpleBallGameLogic::calculateElapsedTime()
{
	return m_fpsTimer.elapsed() * 1000;
}

void SimpleBallGameLogic::onRedraw()
{
	if (m_overalTimePlus < 0)
	{
		m_overalTimePlus = 0;
		long timeElapsedUs = calculateElapsedTime();
		addUs(m_lastS, m_lastUs, timeElapsedUs);
		return;
	}
	lockMutex(m_timestampMutex);
	float path = countPath(m_overalTimePlus, m_ballA);
	path -= countPath(m_overalTimeMinus, m_ballANegative);
	float timeLeft = (m_overalTimeMinus + m_overalTimePlus)/1000000.0f ;
	long timeElapsedUs = calculateElapsedTime();
	float timeElapsedS = timeElapsedUs/1000000.0f;

	if( timeLeft > timeElapsedS)
	{
		timeLeft = 0.0f;
	}
	else
	{
		timeLeft = (timeElapsedS - timeLeft);
	}

	if(m_highState && timeLeft > 0)
	{
		path += countPath(timeLeft, m_ballA);
		path += m_ballV * (m_overalTimePlus + m_overalTimeMinus +  timeLeft);
		m_ballV += timeLeft * m_ballA;
	}
	else if (! m_highState && timeLeft > 0)
	{
		path -= countPath(timeLeft, m_ballANegative);
		path += m_ballV * (m_overalTimePlus + m_overalTimeMinus +  timeLeft);
		m_ballV -= timeLeft * m_ballANegative;
	}
	else if (timeLeft < 0)
	{
		qDebug() << "timestamps are incorrect, expected:" << m_lastS << ", " << m_lastUs;
	}

	m_ballV += m_overalTimePlus * m_ballA;
	m_ballV -= m_overalTimeMinus * m_ballANegative;

	m_overalTimeMinus = 0;
	m_overalTimePlus = 0;
	addUs(m_lastS, m_lastUs, timeElapsedUs);
	m_timestampMutex.unlock();

	lockMutex(m_ballPointMutex);
	
	m_ballPoint.ry() += path;

	if(m_ballPoint.ry() < MIN_HEIGHT)
	{
		m_ballPoint.ry() = MIN_HEIGHT;
		m_ballV = 0.0f;
	}

	if(m_ballPoint.ry() >  MAX_HEIGHT)
	{
		m_ballPoint.ry() = MAX_HEIGHT;
		m_ballV = 0.0f;
	}
	m_ballPointMutex.unlock();
	emit ballMoved(m_ballPoint);

	lockMutex(m_risingMutex);
	m_rising = (path >= 0);
	m_risingMutex.unlock();

	m_fpsTimer.restart();
	qDebug() << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz") << __FUNCTION__;
	emit redrawGame();
}


void SimpleBallGameLogic::addUs(long& toS, long& toUs, const long us)
{
	long usAdd = us;
	while( toUs + usAdd > 1000000)
	{
		toS++;
		toUs += us - 1000000;
		usAdd -= 1000000;
	}

	toUs += us;
}

void SimpleBallGameLogic::lockMutex(QMutex& mutex)
{
	while (! mutex.tryLock())
	{
		qApp->processEvents(QEventLoop::AllEvents,10);
	}
}
