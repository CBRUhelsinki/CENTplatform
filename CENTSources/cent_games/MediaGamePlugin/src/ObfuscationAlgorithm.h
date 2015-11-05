#ifndef OBFUSCATION_ALGORITHM_H 
#define OBFUSCATION_ALGORITHM_H

#include "IAlgorithm.h"
#include "MediaGameDataTypes.h"
#include <QList>
#include <QMutex>

class ObfuscationAlgorithm: public IAlgorithm
{
	Q_OBJECT
	Q_INTERFACES(IAlgorithm)
public:
	ObfuscationAlgorithm(MediaGameDataType* mediaGameData);
	virtual ~ObfuscationAlgorithm();

public slots:
	//from IAlgorithm
	// This method starts the algorithms for game.
	virtual void onStart();

	// Called when the game is ended from CENT.
	virtual void onStop();

	// Called each time when the data comes from device.
	virtual void onUserInput(CentData::DigitalData data);

	// Called one time. Sets the expected input.
	virtual void onExpectedInput(const CentData::DigitalData& data);

	// Called each time when obfuscation change.
	virtual void onTimeout();

signals:
	// Needed by the observer to alpha levle and clipping frame upade.
	void obfuscationUpdated(int alphaLevel, int clippingFrame);

private:
	int updateObfuscationLevel();
	int updateAlphaLevel(int obfuscationLevel);
	int updateClippingFrameLevel(int obfuscationLevel);
	void reset();

private:
	MediaGameDataType* m_mediaGameDataType;
	float              m_currentObfuscation;
	QList<int>         m_classifications;
	QMutex             m_mutex;
	int                m_alphaLevel;
	int                m_clippingFrame;
	int                m_state;
};
#endif //OBFUSCATION_ALGORITHM_H
