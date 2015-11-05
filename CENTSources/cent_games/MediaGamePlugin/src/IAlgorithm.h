#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "CentDataTypes.h"

#include <QObject>
/*
	This virtual class is an interface for new algorithm in CENT Media Game Plugin.
*/
class IAlgorithm : public QObject
{
	Q_OBJECT
public:
	virtual ~IAlgorithm() {};

public slots:
	// This method starts the algorithms for game.
	virtual void onStart() = 0;

	// Called when the game is ended from CENT.
	virtual void onStop() = 0;

	// Called each time when the data comes from device.
	virtual void onUserInput(CentData::DigitalData data) = 0;

	// Called one time. Sets the expected input.
	virtual void onExpectedInput(const CentData::DigitalData& data) = 0;

	// Called each time when obfuscation change.
	virtual void onTimeout() = 0;
};

Q_DECLARE_INTERFACE(IAlgorithm, "cent.game.algorithm");

#endif // ALGORITHM_H
