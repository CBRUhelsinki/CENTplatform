#ifndef TEST_POWER_SPECTRUM_FILE_READER_H
#define TEST_POWER_SPECTRUM_FILE_READER_H

#include <QObject>

#include "PowerSpectrumData.h"

class test_PowerSpectrumFileReader: public QObject
{
	Q_OBJECT
public:
	explicit test_PowerSpectrumFileReader(QObject* parent = 0);
	virtual ~test_PowerSpectrumFileReader();

private slots:
	void construction();
	void readContent();
	void readContentComplexSuccess();
	void readContentComplexSuccess_data();
	void readContentComplexFail();
	void readContentComplexFail_data();
	void processHeaderLine();
	void processLineSuccess();
	void processLineSuccess_data();
	void processLineFail();
	void processLineFail_data();
};

#endif // TEST_POWER_SPECTRUM_FILE_READER_H
