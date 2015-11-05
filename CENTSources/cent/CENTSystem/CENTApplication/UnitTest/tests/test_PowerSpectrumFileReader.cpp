#include "test_PowerSpectrumFileReader.h"
#include "PowerSpectrumFileReader.h"

#include <QTest>
#include <QTextStream>
#include <QDebug>

class PowerSpectrumFileReader_testable: public PowerSpectrumFileReader
{
public:
	explicit PowerSpectrumFileReader_testable(PowerSpectrumData& data)
		: PowerSpectrumFileReader(data)
	{
	}

	virtual ~PowerSpectrumFileReader_testable()
	{
	}

	using PowerSpectrumFileReader::readContent;
	using PowerSpectrumFileReader::processHeaderLine;
	using PowerSpectrumFileReader::readData;
	using PowerSpectrumFileReader::processLine;
};

test_PowerSpectrumFileReader::test_PowerSpectrumFileReader(QObject* parent)
	: QObject(parent)
{
}

test_PowerSpectrumFileReader::~test_PowerSpectrumFileReader()
{
}

void test_PowerSpectrumFileReader::construction()
{
	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);

	PowerSpectrumFileReader_testable* powerSpectrumReaderPtr = new PowerSpectrumFileReader_testable(data);
	delete powerSpectrumReaderPtr;
}

void test_PowerSpectrumFileReader::readContent()
{
	QString string;
	QTextStream streamWrite_0(&string);
	streamWrite_0 << "time; freq; freq" << "\n";
	streamWrite_0 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0" << "\n";
	QTextStream streamRead_0(&string);

	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);

	// fails because of invalid header line
	QCOMPARE(powerSpectrumReader.readContent(streamRead_0), false);

	string = "";
	QTextStream streamWrite_1(&string);
	streamWrite_1 <<"time; c1; c2; c3; freq; freq\n";
	streamWrite_1 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	QTextStream streamRead_1(&string);
	QCOMPARE(powerSpectrumReader.readContent(streamRead_1), true);

	string = "";
	QTextStream streamWrite_2(&string);
	streamWrite_2 <<"time; c1; c2; c3; freq; freq\n";
	QTextStream streamRead_2(&string);
	QCOMPARE(powerSpectrumReader.readContent(streamRead_2), true);
}

void test_PowerSpectrumFileReader::readContentComplexSuccess()
{
	QFETCH(QString, content);
	QTextStream readStream(&content);

	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);

	QTEST(powerSpectrumReader.readContent(readStream), "result");
	QTEST(data.recordCount(), "rows");
	QTEST(data.record(0, 0).m_value, "c0r0_value");
	QTEST(data.record(0, 1).m_value, "c0r1_value");
	QTEST(data.record(0, 2).m_value, "c0r2_value");
	QTEST(data.record(0, 3).m_value, "c0r3_value");
	QTEST(data.record(0, 4).m_value, "c0r4_value");

	QTEST(data.record(1, 0).m_value, "c1r0_value");
	QTEST(data.record(1, 1).m_value, "c1r1_value");
	QTEST(data.record(1, 2).m_value, "c1r2_value");
	QTEST(data.record(1, 3).m_value, "c1r3_value");
	QTEST(data.record(1, 4).m_value, "c1r4_value");

	QTEST(data.record(2, 0).m_value, "c2r0_value");
	QTEST(data.record(2, 1).m_value, "c2r1_value");
	QTEST(data.record(2, 2).m_value, "c2r2_value");
	QTEST(data.record(2, 3).m_value, "c2r3_value");
	QTEST(data.record(2, 4).m_value, "c2r4_value");
}

void test_PowerSpectrumFileReader::readContentComplexSuccess_data()
{
	QTest::addColumn<QString>("content");
	QTest::addColumn<double>("c0r0_value");
	QTest::addColumn<double>("c0r1_value");
	QTest::addColumn<double>("c0r2_value");
	QTest::addColumn<double>("c0r3_value");
	QTest::addColumn<double>("c0r4_value");

	QTest::addColumn<double>("c1r0_value");
	QTest::addColumn<double>("c1r1_value");
	QTest::addColumn<double>("c1r2_value");
	QTest::addColumn<double>("c1r3_value");
	QTest::addColumn<double>("c1r4_value");

	QTest::addColumn<double>("c2r0_value");
	QTest::addColumn<double>("c2r1_value");
	QTest::addColumn<double>("c2r2_value");
	QTest::addColumn<double>("c2r3_value");
	QTest::addColumn<double>("c2r4_value");

	QTest::addColumn<int>("rows");
	QTest::addColumn<bool>("result");

	// Ideal case, everything is correct
	QString stringOk_1;
	QTextStream stream_1(&stringOk_1);
	stream_1 << "Time (s);Channel 0;Channel 1;Channel 2;Min frequency band;Max frequency band\n";
	stream_1 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream_1 << "1.0; 1.0; 2.0; 3.0; 1.0; 2.0\n";
	stream_1 << "1.0; 2.0; 3.0; 4.0; 2.0; 3.0\n";
	stream_1 << "1.0; 3.0; 4.0; 5.0; 3.0; 4.0\n";
	stream_1 << "1.0; 4.0; 5.0; 6.0; 4.0; 5.0\n";

	stream_1 << "2.0; 0.0; 0.1; 0.2;;\n";
	stream_1 << "2.0; 1.0; 2.0; 3.0;;\n";
	stream_1 << "2.0; 2.0; 3.0; 4.0;;\n";
	stream_1 << "2.0; 3.0; 4.0; 5.0;;\n";
	stream_1 << "2.0; 4.0; 5.0; 6.0;;\n";

	stream_1 << "3.0; 0.0; 0.1; 0.2;;\n";
	stream_1 << "3.0; 1.0; 2.0; 3.0;;\n";
	stream_1 << "3.0; 2.0; 3.0; 4.0;;\n";
	stream_1 << "3.0; 3.0; 4.0; 5.0;;\n";
	stream_1 << "3.0; 4.0; 5.0; 6.0;;\n";

	QTest::newRow("row_1") << stringOk_1 
		<< 0.0 << 3.0 << 6.0 << 9.0 << 12.0
		<< 0.3 << 6.0 << 9.0 << 12.0 << 15.0
		<< 0.6 << 9.0 << 12.0 << 15.0 << 18.0
		<< 5 << true;

	// Missing samples for last timeStamp, it is ok I guess
	QString stringOk_2;
	QTextStream stream_2(&stringOk_2);
	stream_2 << "Time (s);Channel 0;Channel 1;Channel 2;Min frequency band;Max frequency band\n";
	stream_2 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream_2 << "1.0; 1.0; 2.0; 3.0; 1.0; 2.0\n";
	stream_2 << "1.0; 2.0; 3.0; 4.0; 2.0; 3.0\n";
	stream_2 << "1.0; 3.0; 4.0; 5.0; 3.0; 4.0\n";
	stream_2 << "1.0; 4.0; 5.0; 6.0; 4.0; 5.0\n";

	stream_2 << "2.0; 0.0; 0.1; 0.2;;\n";
	stream_2 << "2.0; 1.0; 2.0; 3.0;;\n";
	stream_2 << "2.0; 2.0; 3.0; 4.0;;\n";
	stream_2 << "2.0; 3.0; 4.0; 5.0;;\n";
	stream_2 << "2.0; 4.0; 5.0; 6.0;;\n";

	QTest::newRow("row_2") << stringOk_2 
		<< 0.0 << 2.0 << 4.0 << 6.0 << 8.0
		<< 0.2 << 4.0 << 6.0 << 8.0 << 10.0
		<< 0.4 << 6.0 << 8.0 << 10.0 << 12.0
		<< 5 << true;
}

void test_PowerSpectrumFileReader::readContentComplexFail()
{
	QFETCH(QString, content);
	QTextStream readStream(&content);

	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);
	QTEST(powerSpectrumReader.readContent(readStream), "result");
	QTEST(data.checkDataIntegrity(), "integrity");
}

void test_PowerSpectrumFileReader::readContentComplexFail_data()
{
	QTest::addColumn<QString>("content");
	QTest::addColumn<bool>("result");
	QTest::addColumn<bool>("integrity");

	QString string;
	QTextStream stream(&string);
	stream << "Time (s);Channel 0;Channel 1;Channel 2;Min frequency band;Max frequency band\n";
	QTest::newRow("row_1") << string << true << true;

	stream << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";

	stream << "3.0; 1.0; 2.0; 3.0;;\n";
	stream << "3.0; 1.0; 2.0; 3.0;;\n";

	stream << "3.0; 1.0; 2.0; 3.0;;\n";
	stream << "3.0; 1.0; 2.0; 3.0;;\n";
	stream << "3.0; 1.0; 2.0; 3.0;;\n";
	QTest::newRow("row_2") << string << true << false;


	string = "";
	QTextStream stream_1(&string);
	stream_1 << "Time (s);Channel 0;Channel 1;Channel 2;Min frequency band;Max frequency band\n";
	stream_1 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream_1 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";

	stream_1 << "2.0; 1.0; 2.0; 3.0;;\n";
	stream_1 << "2.0; 1.0; 2.0; 3.0;;\n";
	stream_1 << "2.0; 1.0; 2.0; 3.0;;\n";

	stream_1 << "3.0; 1.0; 2.0; 3.0;;\n";
	stream_1 << "3.0; 1.0; 2.0; 3.0;;\n";
	stream_1 << "3.0; 1.0; 2.0; 3.0;;\n";
	QTest::newRow("row_3") << string << true << false;

	string = "";
	QTextStream stream_2(&string);
	stream_2 << "Time (s);Channel 0;Channel 1;Channel 2;Min frequency band;Max frequency band\n";
	stream_2 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream_2 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";

	stream_2 << "2.0; 1.0; 2.0; 3.0;;\n";
	stream_2 << "2.0; 1.0; 2.0; 3.0;;\n";
	stream_2 << "2.0; 1.0; 2.0; 3.0;;\n";
	QTest::newRow("row_4") << string << true << false;

	string = "";
	QTextStream stream_3(&string);
	stream_3 << "Time (s);Channel 0;Channel 1;Channel 2;Min frequency band;Max frequency band\n";
	stream_3 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";
	stream_3 << "1.0; 0.0; 0.1; 0.2; 0.0; 1.0\n";

	stream_3 << "2.0; 1.0; abc; 3.0;;\n";
	stream_3 << "2.0; 1.0; 2.0; 3.0;;\n";
	QTest::newRow("row_4") << string << false << false;
}

void test_PowerSpectrumFileReader::processHeaderLine()
{
	QString line_1 = "";
	QString line_2 = "time, channel 1, channel 2, channel 3, min freq, max freq";
	QString line_3 = "time channel 1 channel 2 channel 3 min freq max freq";
	QString line_4 = "time; channel; channel; channel; freq; freq";
	QString line_5 = "time; channel; freq; freq";
	QString line_6 = "time; freq; freq";
	QString line_7 = "time; freq";
	QString line_8 = "time;";

	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);

	bool result = powerSpectrumReader.processHeaderLine(line_1);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 0);

	result = powerSpectrumReader.processHeaderLine(line_2);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 0);

	result = powerSpectrumReader.processHeaderLine(line_3);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 0);

	result = powerSpectrumReader.processHeaderLine(line_4);
	QCOMPARE(result, true);
	QCOMPARE(data.channelCount(), 3);

	// couldn't read header so nothing is changed
	result = powerSpectrumReader.processHeaderLine(line_3);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 3);

	result = powerSpectrumReader.processHeaderLine(line_5);
	QCOMPARE(result, true);
	QCOMPARE(data.channelCount(), 1);

	result = powerSpectrumReader.processHeaderLine(line_6);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 1);

	result = powerSpectrumReader.processHeaderLine(line_7);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 1);

	result = powerSpectrumReader.processHeaderLine(line_8);
	QCOMPARE(result, false);
	QCOMPARE(data.channelCount(), 1);
}

void test_PowerSpectrumFileReader::processLineSuccess()
{
	QString headerLine = "time; channel; channel; channel; freq; freq";
	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);
	powerSpectrumReader.processHeaderLine(headerLine);

	QFETCH(QString, line);
	double t;

	QTEST(powerSpectrumReader.processLine(line, t), "result");
	QTEST(t, "timeStamp");
}

void test_PowerSpectrumFileReader::processLineSuccess_data()
{
	QTest::addColumn<QString>("line");
	QTest::addColumn<double>("timeStamp");
	QTest::addColumn<bool>("result");

	QTest::newRow("row_1") << "1.00; 1.20; 2.30; 4.60; 1.00; 3.00" << 1.0 << true;
	QTest::newRow("row_2") << "2.00; 2.20; 4.30; 6.60;;" <<  2.0 << true;

	// not sure if this should work like that, maybe when one freq is not valid and other is valid
	// we should treat it as a error?
	QTest::newRow("row_3") << "3.00; 0.20; 0.30; 0.60; abc; 3.00" <<  3.0 << true;
	QTest::newRow("row_4") << "4.00; 1.20; 2.30; 4.60; 1.00; abc" <<  4.0 << true;
}

void test_PowerSpectrumFileReader::processLineFail()
{
	QString headerLine = "time; channel; channel; channel; freq; freq";
	PowerSpectrumData data;
	PowerSpectrumFileReader_testable powerSpectrumReader(data);
	powerSpectrumReader.processHeaderLine(headerLine);

	QFETCH(QString, line);
	double t;
	QList<double> channels;

	QTEST(powerSpectrumReader.processLine(line, t), "result");
}

void test_PowerSpectrumFileReader::processLineFail_data()
{
	QTest::addColumn<QString>("line");
	QTest::addColumn<bool>("result");

	QTest::addColumn<QString>("line");
	QTest::newRow("row_1") << "1.00; 1.20; 2.30; 4.60; 1.00" << false;
	QTest::newRow("row_2") << "; 1.20; 2.30; 4.60; 1.00; 3.00" << false;
	QTest::newRow("row_3") << "abc; 1.20; 2.30; 4.60; 1.00; 3.00" << false;
	QTest::newRow("row_4") << "1.00; 1.20; 2.30; 1.00; 3.00" << false;
	QTest::newRow("row_5") << "1.00; 1.20; 3.00" << false;
	QTest::newRow("row_6") << "1.00; 1.20; abc; 4.60; 1.00; 3.00" << false;
}
