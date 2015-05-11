#ifndef Simple_BALLGAME_LOGIC_H
#define Simple_BALLGAME_LOGIC_H

#include <QPoint>
#include <QMutex>
#include <QTime>

#include "CentDataTypes.h"

class QTimer;

class SimpleBallGameLogic: public QObject
{
	Q_OBJECT
public:
	SimpleBallGameLogic(QObject* parent=0);
	~SimpleBallGameLogic();
	void startLogic();
	void startGame();
	const QPointF& ballPoint();
	const bool isGameOver();
	const bool isRising();
	void onUserInput(CentData::DigitalData data);
	void onExpectedInput(const CentData::DigitalData& data);

signals:
	void redrawGame();
	void ballMoved(const QPointF& point);

private slots:
	void onRedraw();
	void onGameEnd();

private:
	bool m_highState;
	bool m_gameOver;
	bool m_expectedHigh;
	bool m_rising;
	bool m_firstInput;

	QPointF m_ballPoint;
	QTimer* m_redrawTimer;
	QTimer* m_gameTimer;

	QMutex m_timestampMutex;
	QMutex m_ballPointMutex;
	QMutex m_risingMutex;
	QMutex m_gameOverMutex;

	float m_ballF;
	float m_ballM;
	float m_gForce;
	float m_ballA;
	float m_ballV;
	float m_ballANegative;

	float m_overalTimePlus;
	float m_overalTimeMinus;

	long m_lastS;
	long m_lastUs;

private:
	float countPath(const float& time, const float& a);
	void countPositionY(const float& plusS, const float minusS, const float& a);
	void addUs(long& toS, long& toUs, const long us);
	void lockMutex(QMutex& mutex);
	long calculateElapsedTime();

    QTime m_fpsTimer;
};

#endif // Simple_BALL_GAME_LOGIC_H
