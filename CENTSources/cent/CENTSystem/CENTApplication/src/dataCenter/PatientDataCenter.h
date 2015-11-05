#ifndef PATIENT_DATA_CENTER_H
#define PATIENT_DATA_CENTER_H

#include <QMap>
#include <QObject>
#include <QProcess>
#include <QPair>

#include "CentPatientData.h"
#include "CentConstants.h"
#include "Cipher.h"

class BackupManager;
class QStringList;

class PatientDataCenter : public QObject
{
	friend class PatientDataCenter_testable;

	Q_OBJECT
public:
	enum DataHandleMethod
	{
		FILE_SYSTEM = 0
	};

public:
	PatientDataCenter(QObject* parent=0);
	~PatientDataCenter();

	QString currentSessionPath() const;
	QString currentPatientPath() const;
	QString currentGamePath() const;

	// Copy files from source directory to destination directory, to filter files you can specify acceptable extensions.
	// Empty extensions means that all files will be copied
	bool copyFiles(const QString& sourceDirectory, const QString& destinationDirectory, const QStringList& extensions);

	// Use patients default Baseline from iep in current session instead of recording new one
	bool useDefaultBaselineFromIEP();

	// Returns patient directory for specified name
	QString patientDirectory(const QString& patientName);

	// Returns patient IEP directory for specified name
	QString patientIEPDirectory(const QString& patientName);

	// Returns true if directory is not empty
	// ToDo:: Improve and check if contains all needed files
	bool isValidIEPDirectory(const QString& directory);

	bool saveCoefficients(const double& beta, const double& theta);

	bool saveEvaluationData(const CentPatient::EvaluationData& data);

	static QString getDataParentDir();

signals:
	void patientDataSaveSuccess();
	void patientDataSaveFailed();	//TODO: next step RRA: having one signal with error code as parameter would be more consistent with Action H?andler that is eventually going to use that class

	void gameDataSaveSuccess();
	void gameDataSaveFailed();	//TODO: next step RRA: having one signal with error code as parameter would be more consistent with Action H?andler that is eventually going to use that class

	void summaryDataSaveSuccess();
	void summaryDataSaveFailed();	//TODO: next step RRA: having one signal with error code as parameter would be more consistent with Action H?andler that is eventually going to use that class

	void currentPatientPathChanged(const QString& path);
	void currentSessionPathChanged(const QString& path);
	void currentGamePathChanged(const QString& path);

	void betaThetaParsed(double beta, double theta);
	void gameScoresLoaded(QList< QPair<QString, double> > data);
	void sessionResultsLoaded(QList< QPair<QString, double> > data);

public slots:
	void onSavePatientData(CentPatient::Data data, DataHandleMethod method = FILE_SYSTEM);
	void onSaveCurrentUserGameData(CentPatient::GameData gameData, DataHandleMethod method = FILE_SYSTEM);
	void onSaveCurrentGameSummaryData(CentPatient::SummaryData summaryData, DataHandleMethod method = FILE_SYSTEM);

	void triggerParseBetaTheta();
	void triggerReloadGamesSummary();
	void triggerReloadSessionsSummary();

	void resetSession();

private slots:
	bool writeSessionScore(const QList<QPair<QString, double> >& data);

private:
	typedef QPair<QString, double> SCORE_TYPE;

	// Read parent data center from registry or use my documents as fallback
	void setDataParentDir();

	// Creates CENT dir and Patients dir, has to be called in constructor
	// to provide those directories for other components
	bool createBaseDirectoryStructure();

	// Returns name of directory for specified patient name
	// Method first searches for directory that match patient name in PatientDataCenterConstants::PATIENTS_DIR
	// if there is no match new directory name is generated
	QString dirForPatient(const QString& patientName);

	// Write collected data to file system
	bool writePatientToFileSystem(const CentPatient::Data& data);
	bool writeGameDataToFileSystem(const CentPatient::GameData& gameData);
	bool writeSummaryDataToFileSystem(const CentPatient::SummaryData& summaryData);

	// Create patient by given data
	bool savePatientSession(CentPatient::Data& data, DataHandleMethod method = FILE_SYSTEM);
	bool readPatient(CentPatient::Data& data, DataHandleMethod method = FILE_SYSTEM);// TODO: implement patient data reading

	// Create patients game data by given input
	bool saveCurrentPatientGameData(CentPatient::GameData& gameData, DataHandleMethod method = FILE_SYSTEM);
	bool readCurrentPatientGameData(CentPatient::GameData& gameData, DataHandleMethod method = FILE_SYSTEM);// TODO: implement patient game data reading

	// Create game summary data by given input
	bool saveCurrentGameSummaryData(CentPatient::SummaryData& summaryData, DataHandleMethod method = FILE_SYSTEM);
	bool readCurrentGameSummaryData(CentPatient::SummaryData& summaryData, DataHandleMethod method = FILE_SYSTEM);// TODO: implement game summary data reading

	bool writeOVBoxSettingFile(CENT::DirectoryId::Id dirId, CENT::FileNameId::Id fileId, const QStringList& settingsList);

	bool getGameScoreFromSummary(const QString& filename, SCORE_TYPE& data);
	bool getSessionScoreFromSummary(const QString& filename, SCORE_TYPE& data);

	bool backupPatientsFolder();

private:
	CentPatient::Data m_currentUser;
	CentPatient::GameData m_currentGame;

	QString   m_dataParentDir;
	QString   m_currentPatientDir;
	QString   m_currentSessionDir;
	QDateTime m_currentSessionDate;
	QString   m_currentSessionGameDir;
	double    m_currentGameBetaCoeff;
	double    m_currentGameThetaCoeff;

	bool m_currentUserSelected;
	bool m_currentGameSelected;

	QMap<CENT::DirectoryId::Id, const QString*> m_directories;
	QMap<CENT::FileNameId::Id,  const QString*> m_filenames;

	// Used to encrypt/decrypt patient's directory name
	Cipher m_cipher;

	// Used to backup patients files
	BackupManager* m_backupManager;
};

#endif //PATIENT_DATA_CENTER_H