#include "PowerSpectrumFileReader.h"
#include "PowerSpectrumData.h"

#include <QDebug>
#include <QFile>
#include <QStringList>

namespace
{
	const QString COLUMN_SEPARATOR         = ";";
	const int NUMBER_OF_NON_CHANNEL_COLUMN = 3;
}

PowerSpectrumFileReader::PowerSpectrumFileReader(PowerSpectrumData& data)
	: m_data(data)
{
}

PowerSpectrumFileReader::~PowerSpectrumFileReader()
{
}

bool PowerSpectrumFileReader::readFile(const QString& filePath)
{
	QFile file(filePath);
	
	if (! file.exists())
	{
		qWarning() << QString("Power spectrum file: %1 doesn't exist!").arg(filePath);
		return false;
	}

	if (! file.open(QIODevice::ReadOnly))
	{
		qWarning() << QString("Couldn't open file: %1 to read!").arg(filePath);
		return false;
	}

	QTextStream textStream(&file);

	return readContent(textStream);
}

bool PowerSpectrumFileReader::readContent(QTextStream& textStream)
{
	if (! processHeaderLine(textStream.readLine()))
	{
		return false;
	}

	return readData(textStream);
}

bool PowerSpectrumFileReader::processHeaderLine(const QString& headerLine)
{
	// Resolve channel count = number of all columns - time, min freq and max freq columns
	// Last column does not have separator, because of that we add 1
	const int channels = headerLine.count(COLUMN_SEPARATOR) - NUMBER_OF_NON_CHANNEL_COLUMN + 1;

	if (0 >= channels)
	{
		qWarning() << QString("Invalid number of channels: %1").arg(channels);
		return false;
	}

	m_data.setChannelCount(channels);

	return true;
}

bool PowerSpectrumFileReader::readData(QTextStream& textStream)
{
	QString line = "not_null";
	double timeStamp = 0;
	double previousTimeStamp = -1;

	while (! line.isNull())
	{
		line = textStream.readLine();
		if (! line.isNull() && !line.isEmpty() && line.size() > 0)
		{
			if (! processLine(line, timeStamp))
			{
				return false;
			}

			if (-1 == previousTimeStamp)
			{
				m_data.addEpoch(timeStamp);
				previousTimeStamp = timeStamp;
			}

			if (previousTimeStamp != timeStamp)
			{
				m_data.addEpoch(timeStamp);
				previousTimeStamp = timeStamp;
			}
		}
	}

	return true;
}

bool PowerSpectrumFileReader::processLine(const QString& line, double& timeStamp)
{
	double minFrequency = 0;
	double maxFrequency = 0;

	const QStringList columns = line.split(COLUMN_SEPARATOR);
	const int columnCount = columns.size();
	const int channelCount = m_data.channelCount();

	if (channelCount != columnCount - NUMBER_OF_NON_CHANNEL_COLUMN)
	{
		qWarning() << "Line is corrupted, number of channels does not match! Line: " << line;
		return false;
	}

	const int minFrequencyIndex = columnCount - 2;
	const int maxFrequencyIndex = columnCount - 1;

	if (minFrequencyIndex < 0 || columnCount <= minFrequencyIndex)
	{
		qWarning() << QString("minFrequencyIndex is not valid: %1, columnCount: %2").arg(minFrequencyIndex).arg(columnCount);
		return false;
	}

	if (maxFrequencyIndex < 0 || columnCount <= maxFrequencyIndex)
	{
		qWarning() << QString("maxFrequencyIndex is not valid: %1, columnCount: %2").arg(maxFrequencyIndex).arg(columnCount);
		return false;
	}

	bool result = false;
	const QString minFrequencyStr = columns.at(minFrequencyIndex);
	const QString maxFrequencyStr = columns.at(maxFrequencyIndex);
	const QString timeStampStr = columns.at(0);

	timeStamp = timeStampStr.toDouble(&result);
	if (! result)
	{
		qWarning() << "Invalid timeStamp: " << timeStampStr;
		return false;
	}

	minFrequency = minFrequencyStr.toDouble(&result);
	if (! result)
	{
		minFrequency = -1;
	}

	maxFrequency = maxFrequencyStr.toDouble(&result);
	if (! result)
	{
		maxFrequency = -1;
	}

	if (minFrequency != -1 && maxFrequency != -1)
	{
		m_data.addFrequencyInterval(minFrequency, maxFrequency);
	}

	QString valueStr;
	double value = 0;
	for (int i = 0; i < channelCount; ++i)
	{
		valueStr = columns.at(i + 1);
		value = valueStr.toDouble(&result);

		if (! result)
		{
			qWarning() << "Cannot convert value for channel " << i << " to double! Value: " << valueStr;
			return false;
		}
		m_data.addChannelValue(i, value);
	}

	return true;
}
