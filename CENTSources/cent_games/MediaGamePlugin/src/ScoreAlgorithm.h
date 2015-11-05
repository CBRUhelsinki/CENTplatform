#ifndef SCORE_ALGORITHM_H 
#define SCORE_ALGORITHM_H

#include "IAlgorithm.h"
#include "MediaGameDataTypes.h"

#include <QMutex>

class ScoreAlgorithm: public IAlgorithm
{
	Q_OBJECT
	Q_INTERFACES(IAlgorithm)
public:
	ScoreAlgorithm(MediaGameDataType* mediaGameDataType);
	virtual ~ScoreAlgorithm();

public slots:
	//from IAlgorithm
	virtual void onStart();
	virtual void onStop();

	// Called each time when the data comes from device.
	virtual void onUserInput(CentData::DigitalData data);

	// Called one time. Sets the expected input.
	virtual void onExpectedInput(const CentData::DigitalData& data);

	// Called each time when obfuscation change.
	virtual void onTimeout();

signals:
	// Needed by the observer to update score
	void scoreChanged(int score);
	void bonusScore(int score);

private:
	int updateScore();

private:
	MediaGameDataType* m_mediaGameDataType;
	QMutex             m_mutex;
	int                m_score;
	int                m_state;
	QList<int>         m_classifications;
};
#endif //SCORE_ALGORITHM_H