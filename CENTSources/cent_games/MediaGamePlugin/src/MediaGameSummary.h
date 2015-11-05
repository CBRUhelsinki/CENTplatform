#ifndef MEDIA_GAME_SUMMARY_H
#define MEDIA_GAME_SUMMARY_H

#include <QObject>

class QTimer;

class MediaGameSummary : public QObject
{
	Q_OBJECT
public:
	MediaGameSummary(QObject *parent=0);
	~MediaGameSummary();

	int getScore();
	int getAlphaLevel();
	int getClippingFrame();
	int getBonusscore();
	void reset();

public slots:
	// This method update score.
	void onScoreUpdate(int score);

	// This method show extra score
	void onBonusScore(int score);

	// This method update alpha level and clipping frame relation with obfuscation.
	void onObfuscationUpdate(int alphaLevel, int clippingFrame);

private slots:
	void onTimeout();
	void onBonusScoreTimeout();

private:
	void updateClippingFrame();
	void updateAlphaLevel();

private:
	int     m_score;
	int     m_bonusScore;
	int     m_alphaLevel;
	int     m_currentClippingFrame;
	int     m_setClippingFrame;
	int     m_setAlphaLevel;
	QTimer* m_tickTimer;
	QTimer* m_bonusScoreTimer;
};

#endif // MEDIA_GAME_SUMMARY_H
