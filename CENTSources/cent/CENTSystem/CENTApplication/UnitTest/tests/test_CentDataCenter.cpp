#include "test_CentDataCenter.h"

#include <QDateTime>
#include <QTest>
#include <QDir>
#include <QFile>

#include "CentPatientData.h"
#include "PatientDataCenterConfig.h"
#include "PatientDataCenter.h"
#include <QSettings>

class PatientDataCenter_testable: public PatientDataCenter
{
public:
	using PatientDataCenter::dirForPatient;
};

void test_CentDataCenter::construction()
{
	PatientDataCenter test;
	delete new PatientDataCenter();
}
void test_CentDataCenter::savePatientSession()
{
	CentPatient::Data testData;
	testData.m_userName = "John Doe";
	testData.m_hoursSlept = 1;
	testData.m_hoursSinceLastSleep = 2;
	testData.m_motivation = 3;
	testData.m_mood = 4;
	testData.m_excitement = 5;
	testData.m_date = QDateTime::currentDateTime();
	PatientDataCenter_testable test;

	test.onSavePatientData(testData);

	QDir dir = QDir::current();

	//read my documents from registry
	QSettings settings(PatientDataCenterConstants::MY_DOCUMENTS_REGISTRY_KEY,
		               QSettings::NativeFormat);
	QString myDocuments = settings.value(PatientDataCenterConstants::MY_DOCUMENTS_REGISTRY_VALUE).toString();

	QString baseDir = myDocuments 
		            + QDir::separator()
		            + PatientDataCenterConstants::BASE_DIR;

	QString patientsMainDirectory = baseDir
		                          + QDir::separator()
		                          + PatientDataCenterConstants::PATIENTS_DIR;

	QString patientDirectory = patientsMainDirectory
		                     + QDir::separator()
							 + PatientDataCenterConstants::PATIENT_DIRECTORY_FORMATTER.arg(test.dirForPatient(testData.m_userName));

	// Create paths
	QString dateString = testData.m_date.toString(PatientDataCenterConstants::DATE_FORMATTER);

	QString patientSessionFolder = patientDirectory
		                         + QDir::separator()
		                         + PatientDataCenterConstants::PATIENT_SESSION_DIRECTORY_FORMATTER.arg(dateString);

	QString patiensSessionFilePath = patientSessionFolder
		                           + QDir::separator()
		                           + PatientDataCenterConstants::PATIENT_DATA_FILENAME;

	//check created files and directories
	QVERIFY2(dir.exists( baseDir), "Base data directory was not created");

	QVERIFY2(dir.exists( patientsMainDirectory ), "Base patients data directory was not created");

	QVERIFY2(dir.exists(patientDirectory), "Patient John Doe directory is not present");

	QVERIFY2(dir.exists(patientSessionFolder), "Patient John Doe has no session directory");

	QVERIFY2(QFile::exists(patiensSessionFilePath), "patient_condition.txt for patient John Doe not present for created session");
}

void test_CentDataCenter::savePatientSessionGameData()
{//TODO
}

void test_CentDataCenter::setCurrentUser()
{//TODO
}