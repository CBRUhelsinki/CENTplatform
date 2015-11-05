#ifndef ALGORITHM_MANAGER_H
#define ALGORITHM_MANAGER_H

#include "IAlgorithm.h"
#include "MediaGameDataTypes.h"

#include <QSet>

class QTimer;

class AlgorithmManager: public QObject
{
	Q_OBJECT
public:
	AlgorithmManager(const MediaGameDataType& mediaGameDataType);
	virtual ~AlgorithmManager();

	void start();
	void stop();
	void onUserInput(CentData::DigitalData data);
	void onExpectedInput(const CentData::DigitalData& data);

	void addAlgorithm(IAlgorithm* algorithm);

private slots:
	void onTimeout();

private:
	QTimer*               m_obfuscationTimer;
	QSet<IAlgorithm*>     m_algorithms;
};
#endif //ALGORITHM_MANAGER_H
