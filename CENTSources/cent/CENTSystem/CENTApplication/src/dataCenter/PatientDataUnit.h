#ifndef PATIENTDATAUNIT_H
#define PATIENTDATAUNIT_H

#include <QSettings>

class PatientDataUnit : public QSettings
{
	Q_OBJECT
public:
	explicit PatientDataUnit(const QString &fileName, QObject *parent = 0);

private:
	static Format m_format;
};

#endif // PATIENTDATAUNIT_H
