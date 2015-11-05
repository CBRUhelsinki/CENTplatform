#ifndef TEST_POWER_SPECTRUM_DATA_H
#define TEST_POWER_SPECTRUM_DATA_H

#include <QObject>

class test_PowerSpectrumData: public QObject
{
	Q_OBJECT
public:
	explicit test_PowerSpectrumData(QObject* parent = 0);
	virtual ~test_PowerSpectrumData();

private slots:
	void construction();
	void channelCount();
	void recordCount();
	void epochCount();
	void records();
	void addChannelValue();
	void lastRowForFrequency();
};

#endif // TEST_POWER_SPECTRUM_DATA_H
