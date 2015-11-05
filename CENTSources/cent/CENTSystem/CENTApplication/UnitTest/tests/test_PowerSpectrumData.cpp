#include "test_PowerSpectrumData.h"
#include "PowerSpectrumData.h"

#include <QTest>

bool operator==(const PowerSpectrumRecord& left, const PowerSpectrumRecord& right)
{
	return qFuzzyCompare(left.m_value, right.m_value) && qFuzzyCompare(left.m_minFrequency, right.m_minFrequency)
		&& qFuzzyCompare(left.m_maxFrequency, right.m_maxFrequency);
}

class PowerSpectrumData_testable: public PowerSpectrumData
{
public:
	using PowerSpectrumData::m_data;
};

test_PowerSpectrumData::test_PowerSpectrumData(QObject* parent)
	: QObject(parent)
{
}

test_PowerSpectrumData::~test_PowerSpectrumData()
{
}

void test_PowerSpectrumData::construction()
{
	PowerSpectrumData_testable powerSpectrumData;

	PowerSpectrumData_testable* powerSpectrumDataPtr = new PowerSpectrumData_testable();
	delete powerSpectrumDataPtr;
}

void test_PowerSpectrumData::channelCount()
{
	PowerSpectrumData_testable powerSpectrumData;
	QCOMPARE(0, powerSpectrumData.channelCount());

	powerSpectrumData.setChannelCount(-5);
	QCOMPARE(0, powerSpectrumData.channelCount());

	powerSpectrumData.setChannelCount(1);
	powerSpectrumData.addEpoch(1.0);
	powerSpectrumData.addFrequencyInterval(0.0, 1.0);
	powerSpectrumData.addChannelValue(0, 1.0);
	QCOMPARE(true, powerSpectrumData.checkDataIntegrity());

	// After channels are set, list have to be cleared
	powerSpectrumData.setChannelCount(5);
	QCOMPARE(powerSpectrumData.channelCount(), 5);
	QCOMPARE(powerSpectrumData.recordCount(), 0);
	QCOMPARE(powerSpectrumData.epochs().size(), 0);
	QCOMPARE(powerSpectrumData.m_data.m_minFrequencies.size(), 0);
	QCOMPARE(powerSpectrumData.m_data.m_maxFrequencies.size(), 0);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(0).size(), 0);


	powerSpectrumData.setChannelCount(15);
	QCOMPARE(15, powerSpectrumData.channelCount());
	QCOMPARE(0, powerSpectrumData.recordCount());

	powerSpectrumData.setChannelCount(0);
	QCOMPARE(0, powerSpectrumData.channelCount());
	QCOMPARE(0, powerSpectrumData.recordCount());
}

void test_PowerSpectrumData::recordCount()
{
	PowerSpectrumData_testable powerSpectrumData;
	QCOMPARE(0, powerSpectrumData.recordCount());

	powerSpectrumData.addFrequencyInterval(0, 1);
	QCOMPARE(1, powerSpectrumData.recordCount());

	powerSpectrumData.setChannelCount(1);
	QCOMPARE(0, powerSpectrumData.recordCount());

	powerSpectrumData.addFrequencyInterval(1, 2);
	QCOMPARE(1, powerSpectrumData.recordCount());

	powerSpectrumData.addFrequencyInterval(2, 3);
	powerSpectrumData.addFrequencyInterval(3, 4);
	powerSpectrumData.addFrequencyInterval(4, 5);
	QCOMPARE(4, powerSpectrumData.recordCount());

	powerSpectrumData.setChannelCount(1);
	QCOMPARE(0, powerSpectrumData.recordCount());
}

void test_PowerSpectrumData::epochCount()
{
	PowerSpectrumData_testable powerSpectrumData;
	QCOMPARE(0, powerSpectrumData.epochCount());

	powerSpectrumData.addEpoch(1.0);
	QCOMPARE(1, powerSpectrumData.epochCount());
	QVERIFY(qFuzzyCompare(1.0, powerSpectrumData.epochs().at(0)));

	// Adding the same epoch value, nothing will change
	powerSpectrumData.addEpoch(1.0);
	QCOMPARE(1, powerSpectrumData.epochCount());
	QVERIFY(qFuzzyCompare(1.0, powerSpectrumData.epochs().at(0)));

	powerSpectrumData.addEpoch(2.0);
	powerSpectrumData.addEpoch(3.0);
	powerSpectrumData.addEpoch(4.0);
	QCOMPARE(4, powerSpectrumData.epochCount());
	QVERIFY(qFuzzyCompare(1.0, powerSpectrumData.epochs().at(0)));
	QVERIFY(qFuzzyCompare(2.0, powerSpectrumData.epochs().at(1)));
	QVERIFY(qFuzzyCompare(3.0, powerSpectrumData.epochs().at(2)));
	QVERIFY(qFuzzyCompare(4.0, powerSpectrumData.epochs().at(3)));

	// Adding the same epoch value, nothing will change
	powerSpectrumData.addEpoch(2.0);
	QCOMPARE(4, powerSpectrumData.epochCount());
}

void test_PowerSpectrumData::records()
{
	PowerSpectrumData_testable powerSpectrumData;
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 0));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 0, 0));
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), true);

	powerSpectrumData.setChannelCount(1);
	powerSpectrumData.addFrequencyInterval(0, 1);
	powerSpectrumData.addChannelValue(0, 1);
	powerSpectrumData.addEpoch(0);
	QCOMPARE(PowerSpectrumRecord(1, 0, 1), powerSpectrumData.record(0, 0));
	QCOMPARE(PowerSpectrumRecord(1, 0, 1), powerSpectrumData.record(0, 0, 0));
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), true);

	powerSpectrumData.addFrequencyInterval(1, 2);
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), false);
	powerSpectrumData.addChannelValue(0, 2);
	QCOMPARE(PowerSpectrumRecord(2, 1, 2), powerSpectrumData.record(0, 1));
	QCOMPARE(PowerSpectrumRecord(2, 1, 2), powerSpectrumData.record(0, 1, 0));
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), true);

	powerSpectrumData.addFrequencyInterval(2,3);
	powerSpectrumData.addChannelValue(0, 3);
	QCOMPARE(PowerSpectrumRecord(3, 2, 3), powerSpectrumData.record(0, 2));
	QCOMPARE(PowerSpectrumRecord(3, 2,3), powerSpectrumData.record(0, 2, 0));
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), true);

	powerSpectrumData.addEpoch(1);
	powerSpectrumData.addChannelValue(0, 1);
	powerSpectrumData.addChannelValue(0, 2);
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), false);
	powerSpectrumData.addChannelValue(0, 3);

	QCOMPARE(PowerSpectrumRecord(2, 0, 1), powerSpectrumData.record(0, 0));
	QCOMPARE(PowerSpectrumRecord(1, 0, 1), powerSpectrumData.record(0, 0, 0));
	QCOMPARE(PowerSpectrumRecord(1, 0, 1), powerSpectrumData.record(0, 0, 1));

	QCOMPARE(PowerSpectrumRecord(4, 1, 2), powerSpectrumData.record(0, 1));
	QCOMPARE(PowerSpectrumRecord(2, 1, 2), powerSpectrumData.record(0, 1, 0));
	QCOMPARE(PowerSpectrumRecord(2, 1, 2), powerSpectrumData.record(0, 1, 1));

	QCOMPARE(PowerSpectrumRecord(6, 2, 3), powerSpectrumData.record(0, 2));
	QCOMPARE(PowerSpectrumRecord(3, 2, 3), powerSpectrumData.record(0, 2, 0));
	QCOMPARE(PowerSpectrumRecord(3, 2, 3), powerSpectrumData.record(0, 2, 1));
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), true);

	powerSpectrumData.addEpoch(2);
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), false);
	powerSpectrumData.addChannelValue(0, 3);
	powerSpectrumData.addChannelValue(0, 4);
	powerSpectrumData.addChannelValue(0, 5);
	QCOMPARE(PowerSpectrumRecord(5, 0, 1), powerSpectrumData.record(0, 0));
	QCOMPARE(PowerSpectrumRecord(1, 0, 1), powerSpectrumData.record(0, 0, 0));
	QCOMPARE(PowerSpectrumRecord(1, 0, 1), powerSpectrumData.record(0, 0, 1));
	QCOMPARE(PowerSpectrumRecord(3, 0, 1), powerSpectrumData.record(0, 0, 2));

	QCOMPARE(PowerSpectrumRecord(8, 1, 2), powerSpectrumData.record(0, 1));
	QCOMPARE(PowerSpectrumRecord(2, 1, 2), powerSpectrumData.record(0, 1, 0));
	QCOMPARE(PowerSpectrumRecord(2, 1, 2), powerSpectrumData.record(0, 1, 1));
	QCOMPARE(PowerSpectrumRecord(4, 1, 2), powerSpectrumData.record(0, 1, 2));

	QCOMPARE(PowerSpectrumRecord(11, 2, 3), powerSpectrumData.record(0, 2));
	QCOMPARE(PowerSpectrumRecord(3, 2, 3), powerSpectrumData.record(0, 2, 0));
	QCOMPARE(PowerSpectrumRecord(3, 2, 3), powerSpectrumData.record(0, 2, 1));
	QCOMPARE(PowerSpectrumRecord(5, 2, 3), powerSpectrumData.record(0, 2, 2));
	QCOMPARE(powerSpectrumData.checkDataIntegrity(), true);

	// Invalid epochs
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 0, -3));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 1, 4));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 2, 5));

	// Invalid channels
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(-1, 0, 0));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(5, 1, 1));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(15, 2, 2));

	// Invalid rows
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, -1, 0));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 4, 1));
	QCOMPARE(PowerSpectrumRecord(), powerSpectrumData.record(0, 8, 2));
}

void test_PowerSpectrumData::addChannelValue()
{
	PowerSpectrumData_testable powerSpectrumData;
	powerSpectrumData.setChannelCount(2);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(0).size(), 0);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(1).size(), 0);

	// Invalid channel
	powerSpectrumData.addChannelValue(5, 9);
	powerSpectrumData.addChannelValue(-5, 9);

	powerSpectrumData.addChannelValue(0, 8);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(0).size(), 1);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(0).contains(8), true);

	powerSpectrumData.addChannelValue(0, 18);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(0).size(), 2);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(0).contains(18), true);

	powerSpectrumData.addChannelValue(1, 3);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(1).size(), 1);
	QCOMPARE(powerSpectrumData.m_data.m_channelValues.at(1).contains(3), true);
}

void test_PowerSpectrumData::lastRowForFrequency()
{
	PowerSpectrumData_testable powerSpectrumData;
	powerSpectrumData.setChannelCount(1);
	
	// Record count is 0
	QCOMPARE(-1, powerSpectrumData.lastRowForFrequency(0.5));

	powerSpectrumData.addFrequencyInterval(0, 1);
	powerSpectrumData.addFrequencyInterval(1, 2);
	powerSpectrumData.addFrequencyInterval(2, 3);
	powerSpectrumData.addFrequencyInterval(3, 4);
	powerSpectrumData.addFrequencyInterval(4, 5);
	powerSpectrumData.addFrequencyInterval(5, 6);

	// Ok
	QCOMPARE(1, powerSpectrumData.lastRowForFrequency(2));

	// Ok
	QCOMPARE(3, powerSpectrumData.lastRowForFrequency(4));

	// Specified frequency below frequency in first sample
	QCOMPARE(-1, powerSpectrumData.lastRowForFrequency(0.5));

	// Ok
	QCOMPARE(5, powerSpectrumData.lastRowForFrequency(8));
}