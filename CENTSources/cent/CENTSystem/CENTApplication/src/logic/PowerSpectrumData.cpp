#include "PowerSpectrumData.h"

#include <QDebug>

PowerSpectrumData::PowerSpectrumData()
{
}

PowerSpectrumData::~PowerSpectrumData()
{
}

void PowerSpectrumData::setChannelCount(int channelCount)
{
	m_data.m_channelValues.clear();
	m_data.m_minFrequencies.clear();
	m_data.m_maxFrequencies.clear();
	m_data.m_epochs.clear();

	for (int i = 0; i < channelCount; ++i)
	{
		m_data.m_channelValues.append(QVector<double>());
	}
}

int PowerSpectrumData::channelCount() const
{
	return m_data.m_channelValues.size();
}

int PowerSpectrumData::recordCount() const
{
	return m_data.m_minFrequencies.size();
}

int PowerSpectrumData::epochCount() const
{
	return m_data.m_epochs.size();
}

void PowerSpectrumData::addEpoch(double epoch)
{
	if (m_data.m_epochs.contains(epoch))
	{
		qWarning() << "Adding the same epoch second time, epoch: " << epoch;
		return;
	}
	m_data.m_epochs.append(epoch);
}

const QVector<double>& PowerSpectrumData::epochs() const
{
	return m_data.m_epochs;
}

PowerSpectrumRecord PowerSpectrumData::record(int channel, int row) const
{
	if (0 > channel || channel >= channelCount())
	{
		qWarning() << "Invalid channel: " << channel;
		return PowerSpectrumRecord();
	}

	const int rows = recordCount();
	if (0 > row || row >= rows)
	{
		qWarning() << "Invalid row: " << row;
		return PowerSpectrumRecord();
	}

	PowerSpectrumRecord record;

	record.m_maxFrequency = m_data.m_maxFrequencies.at(row);
	record.m_minFrequency = m_data.m_minFrequencies.at(row);
	record.m_value = 0;

	const int epochs = m_data.m_epochs.size();

	int index = 0;
	for (int i = 0; i < epochs; ++i)
	{
		index = i * rows + row;
		if (m_data.m_channelValues.at(channel).size() > index)
		{
			record.m_value += m_data.m_channelValues.at(channel).at(index);
		}
		else
		{
			qWarning() << "Invalid index: " << index;
			return PowerSpectrumRecord();
		}
	}

	return record;
}

PowerSpectrumRecord PowerSpectrumData::record(int channel, int row, int epoch) const
{
	if (0 > channel || channel >= channelCount())
	{
		qWarning() << "Invalid channel: " << channel;
		return PowerSpectrumRecord();
	}

	const int rows = recordCount();
	if (0 > row || row >= rows)
	{
		qWarning() << "Invalid row: " << row;
		return PowerSpectrumRecord();
	}

	if (0 > epoch || epoch >= epochCount())
	{
		qWarning() << "Invalid epoch: " << epoch;
		return PowerSpectrumRecord();
	}

	PowerSpectrumRecord record;
	int index = 0;
	if (m_data.m_channelValues.at(channel).size() > index)
	{
		record.m_maxFrequency = m_data.m_maxFrequencies.at(row);
		record.m_minFrequency = m_data.m_minFrequencies.at(row);
		record.m_value = m_data.m_channelValues.at(channel).at(epoch * recordCount() + row);
	}
	else
	{
		qWarning() << "Invalid index: " << index;
	}

	return record;
}

int PowerSpectrumData::lastRowForFrequency(double frequency) const
{
	const int records = recordCount();
	int i = 0;

	if (0 < records)
	{
		for (i = 0; i < records; ++i)
		{
			if (frequency < m_data.m_maxFrequencies.at(i))
			{
				break;
			}
		}
	}
	else
	{
		qWarning() << "Record count is " << records;
	}

	return i - 1;
}

void PowerSpectrumData::addFrequencyInterval(double minFrequency, double maxFrequency)
{
	m_data.m_minFrequencies.append(minFrequency);
	m_data.m_maxFrequencies.append(maxFrequency);
}

void PowerSpectrumData::addChannelValue(int channel, double value)
{
	if (0 > channel || channel >= channelCount())
	{
		qWarning() << "There is no requested channel, number: " << channel;
		return;
	}

	m_data.m_channelValues[channel].append(value);
}

bool PowerSpectrumData::checkDataIntegrity() const
{
	const int minFrequencyCount = m_data.m_minFrequencies.size();
	const int maxFrequencyCount = m_data.m_maxFrequencies.size();
	const int epochCount = m_data.m_epochs.size();
	const int expectedRowsCount = epochCount * minFrequencyCount;

	if (minFrequencyCount != maxFrequencyCount)
	{
		qWarning() << "Frequencies does not match!";
		return false;
	}

	const int channels = m_data.m_channelValues.size();
	int rows = 0;
	for (int i = 0; i < channels; ++i)
	{
		rows = m_data.m_channelValues.at(i).size();
		if (rows != expectedRowsCount)
		{
			qWarning() << "Invalid number of rows for chanel: " << i << ", rows: " << rows;
			return false;
		}
	}

	return true;
}