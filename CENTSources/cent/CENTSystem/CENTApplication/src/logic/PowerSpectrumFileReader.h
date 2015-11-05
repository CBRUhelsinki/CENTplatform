#ifndef POWER_SPECTRUM_FILE_READER_H
#define POWER_SPECTRUM_FILE_READER_H

#include <QString>
#include <QMap>

class PowerSpectrumData;
class QTextStream;

class PowerSpectrumFileReader
{
	friend class PowerSpectrumFileReader_testable;

public:
	explicit PowerSpectrumFileReader(PowerSpectrumData& data);
	~PowerSpectrumFileReader();

public:
	// Reads csv file with spectrum data
	bool readFile(const QString& filePath);
	
private:
	// Reads content of csv file
	bool readContent(QTextStream& textStream);

	// Process header line, checks number of channels
	bool processHeaderLine(const QString& headerLine);

	// Reads data from file, used after processing header line
	bool readData(QTextStream& textStream);

	// Process single line
	bool processLine(const QString& line, double& timeStamp);

private:
	PowerSpectrumFileReader(const PowerSpectrumFileReader&);
	PowerSpectrumFileReader& operator=(const PowerSpectrumFileReader&);

	PowerSpectrumData& m_data;
};
#endif // POWER_SPECTRUM_FILE_READER_H
