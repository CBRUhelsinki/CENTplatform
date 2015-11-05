#ifndef POWER_SPECTRUM_DATA_H
#define POWER_SPECTRUM_DATA_H

#include <QVector>

struct PowerSpectrumRecord
{
	// Contains sum of values for all epochs
	// or value for specified epoch. Depends on retrieve method
	// from PowerSpectrumData
	double m_value;

	double m_minFrequency;
	double m_maxFrequency;

	PowerSpectrumRecord()
		: m_value(0)
		, m_minFrequency(-1)
		, m_maxFrequency(-1)
	{
	}

	PowerSpectrumRecord(double value, double minFrequency, double maxFrequency)
		: m_value(value)
		, m_minFrequency(minFrequency)
		, m_maxFrequency(maxFrequency)
	{
	}
};

class PowerSpectrumData
{
	friend class PowerSpectrumData_testable;

	struct PowerSpectrumInternalData
	{
		QVector<double> m_epochs;
		QVector<double> m_minFrequencies;
		QVector<double> m_maxFrequencies;
		QVector<QVector<double> > m_channelValues;
	};

public:
	PowerSpectrumData();
	~PowerSpectrumData();

public:
	// Sets number of channels
	void setChannelCount(int channelCount);

	// Returns number of channels
	int channelCount() const;

	// Returns number of records
	int recordCount() const;

	// Returns number of epochs
	int epochCount() const;

	// Returns epochs as a vector
	const QVector<double>& epochs() const;

	// Returns record for specified channel and row
	PowerSpectrumRecord record(int channel, int row) const;

	// Returns record for specified channel, row and epoch
	PowerSpectrumRecord record(int channel, int row, int epoch) const;

	// Return number of row in the list which max Frequency is lower than specified
	int lastRowForFrequency(double frequency) const;

	// Adds epoch to the epoch list
	void addEpoch(double epoch);

	// Adds frequency interval
	void addFrequencyInterval(double minFrequency, double maxFrequency);

	// Adds value for specified channel
	void addChannelValue(int channel, double value);

	// Returns true if data is valid, otherwise returns false
	bool checkDataIntegrity() const;

private:
	PowerSpectrumData(const PowerSpectrumData&);
	PowerSpectrumData& operator=(const PowerSpectrumData&);

	PowerSpectrumInternalData m_data;
};
#endif // POWER_SPECTRUM_DATA_H
