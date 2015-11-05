#include "SummaryController.h"

SummaryController::SummaryController(QObject* parent)
	: QObject(parent)
	, m_duringGame(false)
{
	resetTimeStamps();
}

SummaryController::~SummaryController()
{
}

const QList<SummaryData>& SummaryController::userInputData() const
{
	return m_userInputData;
}

const QList<SummaryData>& SummaryController::expectedInputData() const
{
	return m_expectedInputData;
}

const double SummaryController::sessionTime() const
{
	return m_sessionTime;
}

// ToDo:: it has to be rewritten, it works only for constant expected input 1
const double SummaryController::score()
{
	int state = 0;
	int duration = 0;
	double score = 0;

	const int size = m_userInputData.size();
	
	for (int i = 0; i < size; ++i)
	{
		state = m_userInputData.at(i).m_input;
		if (i < size - 1)
		{
			duration = m_userInputData.at(i).m_time.msecsTo(m_userInputData.at(i+1).m_time);
		}
		else
		{
			duration = m_userInputData.at(i).m_time.msecsTo(m_gameFinished);
		}

		if (state == 1)
		{
			score += duration;
		}
	}

	score = score/m_sessionTime * 100;

	return score;
}

void SummaryController::onGameStarted()
{
	resetInputData();

	m_duringGame = true;

	m_gameStarted = QTime::currentTime();
}

void SummaryController::onGameFinished()
{
	m_duringGame = false;

	m_gameFinished = QTime::currentTime();

	m_sessionTime = m_gameStarted.msecsTo(m_gameFinished);

	emit summaryChanged();
}

void SummaryController::onExpectedInput(const CentData::DigitalData& data)
{
	if (m_duringGame)
	{
		addData(m_expectedInputData, data);
	}
}

void SummaryController::onUserInput(const CentData::DigitalData& data)
{
	if (m_duringGame && data.button == CentData::Classification)
	{
		addData(m_userInputData, data);
	}
}

void SummaryController::resetTimeStamps()
{
	m_gameStarted = QTime();
	m_gameFinished = QTime();
	m_sessionTime = 0;
}

void SummaryController::resetInputData()
{
	m_expectedInputData.clear();
	m_userInputData.clear();

	const QTime time = QTime::currentTime();

	m_expectedInputData.append(SummaryData(time, 0));
	m_userInputData.append(SummaryData(time, 0));
}

void SummaryController::addData(QList<SummaryData>& dataContainer, const CentData::DigitalData& data)
{
	if (! dataContainer.isEmpty())
	{
		const SummaryData& lastData = dataContainer.last();
		
		if(lastData.m_input == data.state)
		{
			return;
		}
	}

	dataContainer.append(SummaryData(QTime::currentTime(), static_cast<int>(data.state)));
}


