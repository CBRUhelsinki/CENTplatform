#include "PatientDataUnit.h"

#include <QTextStream>

namespace
{
	const QString extension = "txt";
	const QChar separator = ':';
}
//TODO: add special chars escaping
bool readData( QIODevice& device, QSettings::SettingsMap& map )
{
	QTextStream stream(&device);
	QString line, key, tmp;
	QVariant value;
	while(!stream.atEnd())
	{
		line = stream.readLine();
		int idx = line.indexOf(separator);
		if(idx < 0)
		{
			return false;
		}
		QString escapedKey;
		key = line.left(idx);
		tmp = line.right(line.count() - idx - 1);
		map.insert(key, tmp);
	}

	return true;
}

bool writeData( QIODevice& device, const QSettings::SettingsMap& map )
{
	QTextStream stream(&device);

	QSettings::SettingsMap::const_iterator it;
	for(it = map.begin(); it != map.end(); ++it )
	{
		stream << it.key() << ":" << it.value().toString() << "\r\n" << flush;
	}
	return true;
}

QSettings::Format PatientDataUnit::m_format = QSettings::registerFormat(extension, readData, writeData);


PatientDataUnit::PatientDataUnit(const QString &fileName, QObject *parent) :
	QSettings(fileName, m_format, parent)
{

}
