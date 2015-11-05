#include "AlgorithmManager.h"
#include "Connect.h"
#include "MediaGameDataTypes.h"

#include <QTimer>


AlgorithmManager::AlgorithmManager(const MediaGameDataType& mediaGameDataType)
{
	m_obfuscationTimer = new QTimer(this);
	
	m_obfuscationTimer->setInterval(mediaGameDataType.getTimeoutObfuscation());
	CENT::connect(m_obfuscationTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

AlgorithmManager::~AlgorithmManager()
{
}

void AlgorithmManager::addAlgorithm(IAlgorithm* algorithm)
{
	if(algorithm)
	{
		m_algorithms.insert(algorithm);
	}
}

void AlgorithmManager::start()
{
	m_obfuscationTimer->start();
	foreach (IAlgorithm* algorithm, m_algorithms)
	{
		algorithm->onStart();
	}
}

void AlgorithmManager::stop()
{
	m_obfuscationTimer->stop();
	foreach (IAlgorithm* algorithm, m_algorithms)
	{
		algorithm->onStop();
	}
}

void AlgorithmManager::onUserInput(CentData::DigitalData data)
{
	foreach (IAlgorithm* algorithm, m_algorithms)
	{
		algorithm->onUserInput(data);
	}
}

void AlgorithmManager::onExpectedInput(const CentData::DigitalData& data)
{
	Q_UNUSED(data);
}

void AlgorithmManager::onTimeout()
{
	foreach (IAlgorithm* algorithm, m_algorithms)
	{
		algorithm->onTimeout();
	}
}
