#include "PatientDataCenter.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

#include "BackupManager.h"
#include "CentFileHelpers.h"
#include "CentPatientData.h"
#include "Connect.h"
#include "PatientDataCenterConfig.h"
#include "PatientDataUnit.h"

namespace
{
	bool pathByFileDateLessThan(const QString& one, const QString& two)
	{
		QFileInfo fileOne(one);
		QFileInfo fileTwo(two);

		if(!fileOne.exists())
		{
			return false;
		}
		if(!fileTwo.exists())
		{
			return true;
		}

		return fileOne.created() < fileTwo.created();
	}

	const QString DATE_BAR_CHART_FORMATTER = "h:mm d/MM/yyyy";
}

PatientDataCenter::PatientDataCenter(QObject* parent)
	: QObject(parent)
	, m_currentSessionDir()
	, m_currentUserSelected(false)
	, m_currentGameSelected(false)
	, m_currentGameBetaCoeff(1.0)
	, m_currentGameThetaCoeff(1.0)
{
	setDataParentDir();

	m_directories.insert(CENT::DirectoryId::DirectoryCurrentSession, &m_currentSessionDir);
	m_directories.insert(CENT::DirectoryId::DirectoryCurrentGame,    &m_currentSessionGameDir);

	m_filenames.insert(CENT::FileNameId::FileNameBetaCoeff,  &PatientDataCenterConstants::BETA_COEFF_FILENAME);
	m_filenames.insert(CENT::FileNameId::FileNameThetaCoeff, &PatientDataCenterConstants::THETA_COEFF_FILENAME);

	createBaseDirectoryStructure();

	QString patientsDir = m_dataParentDir + QDir::separator() + PatientDataCenterConstants::BASE_DIR 
		                + QDir::separator() + PatientDataCenterConstants::PATIENTS_DIR;
	m_backupManager = new BackupManager(patientsDir, this);
	m_backupManager->setDestSubdir(PatientDataCenterConstants::PATIENTS_DIR);

	CENT::connect(this, SIGNAL(gameScoresLoaded(QList<QPair<QString, double> >)), this, SLOT(writeSessionScore(QList<QPair<QString, double> >)));
}

PatientDataCenter::~PatientDataCenter()
{
}

QString PatientDataCenter::currentSessionPath() const
{
	return m_currentSessionDir;
}

QString PatientDataCenter::currentPatientPath() const
{
	return m_currentPatientDir;
}

QString PatientDataCenter::currentGamePath() const
{
	return m_currentSessionGameDir;
}

bool PatientDataCenter::copyFiles(const QString& sourceDirectory, const QString& destinationDirectory, const QStringList& extensions)
{
	return CentFileHelpers::copyFiles(sourceDirectory, destinationDirectory, extensions);
}

bool PatientDataCenter::useDefaultBaselineFromIEP()
{
	if(!m_currentUserSelected)
	{
		qCritical() << __FUNCTION__ << "No current user selected";
		return false;
	}

	QString defaultBaselinePath = m_currentPatientDir + QDir::separator() + PatientDataCenterConstants::IEP_DIR
		+ QDir::separator() + PatientDataCenterConstants::DEFAULT_BASELINE_DIR;
	QDir baselineDir(defaultBaselinePath);
	QDir sessionDir(m_currentSessionDir);
	if(!baselineDir.exists() || !sessionDir.exists())
	{
		return false;
	}
	return copyFiles(defaultBaselinePath, m_currentSessionDir, QStringList());
}

QString PatientDataCenter::patientDirectory(const QString& patientName)
{
	return QString("%1%2%3%4").arg(m_dataParentDir)
		.arg(QDir::separator() + PatientDataCenterConstants::BASE_DIR)
		.arg(QDir::separator() + PatientDataCenterConstants::PATIENTS_DIR)
		.arg(QDir::separator() + dirForPatient(patientName));
}

QString PatientDataCenter::patientIEPDirectory(const QString& patientName)
{
	return patientDirectory(patientName) + QDir::separator() + PatientDataCenterConstants::IEP_DIR;
}

bool PatientDataCenter::isValidIEPDirectory(const QString& directory)
{
	QDir dir(directory);
	return !dir.entryInfoList(QDir::Files).isEmpty();
}

bool PatientDataCenter::savePatientSession(CentPatient::Data& data, DataHandleMethod method)
{
	switch(method)//TODO: add a map with DataHandleMethod to write method/object
	{
		case FILE_SYSTEM:
			return writePatientToFileSystem(data);
		break;

		default:
			return false;
		break;
	}
}

bool PatientDataCenter::readPatient(CentPatient::Data& data, DataHandleMethod method)
{
	Q_UNUSED(data);
	switch(method) //TODO: add a map with DataHandleMethod to read method/object mapping
	{
		case FILE_SYSTEM:
			return false; //TODO: add a method for reading patients
		break;

		default:
			return false;
		break;
	}
}

void PatientDataCenter::onSavePatientData(CentPatient::Data data, DataHandleMethod method)
{
	if(savePatientSession(data, method))
	{
		m_currentUser = data;
		m_currentUserSelected = true;
		emit patientDataSaveSuccess();
	}
	else
	{
		m_currentUserSelected = false;
		emit patientDataSaveFailed();
	}
}

void PatientDataCenter::onSaveCurrentUserGameData(CentPatient::GameData gameData, DataHandleMethod method)
{
	if(m_currentUserSelected)
	{
		if(saveCurrentPatientGameData(gameData, method))
		{
			emit gameDataSaveSuccess();
		}
		else
		{
			emit gameDataSaveFailed();
		}
	}
	else
	{
		qCritical() << __FUNCTION__ << "No current user selected";
		emit gameDataSaveFailed();
	}
}

void PatientDataCenter::onSaveCurrentGameSummaryData(CentPatient::SummaryData summaryData, DataHandleMethod method)
{
	if(m_currentGameSelected)
	{
		if(saveCurrentGameSummaryData(summaryData, method))
		{
			emit summaryDataSaveSuccess();
		}
		else
		{
			emit summaryDataSaveFailed();
		}
	}
	else
	{
		qCritical() << __FUNCTION__ << "Current game has not been set.";
		emit summaryDataSaveFailed();
	}
}

bool PatientDataCenter::saveCurrentPatientGameData(CentPatient::GameData& gameData, DataHandleMethod method)
{
	switch(method)//TODO: add a map with DataHandleMethod to write method/object
	{
		case FILE_SYSTEM:
			return writeGameDataToFileSystem(gameData);
		break;

		default:
			qCritical() << __FUNCTION__ << "Unknown save method for patient game data";
			return false;
		break;
	}
}

bool PatientDataCenter::readCurrentPatientGameData(CentPatient::GameData& gameData, DataHandleMethod method)//TODO
{
	Q_UNUSED(gameData);

	switch(method) //TODO: add a map with DataHandleMethod to read method/object mapping
	{
		case FILE_SYSTEM:
			return false; //TODO: add a method for reading game data
		break;

		default:
			qCritical() << __FUNCTION__ << "Unknown save method for patient session data";
			return false;
		break;
	}
}

bool PatientDataCenter::saveCurrentGameSummaryData(CentPatient::SummaryData& summaryData, DataHandleMethod method)
{
	switch(method)//TODO: add a map with DataHandleMethod to write method/object
	{
		case FILE_SYSTEM:
			return writeSummaryDataToFileSystem(summaryData);
		break;

		default:
			qCritical() << __FUNCTION__ << "Unknown save method for patient symmary data";
			return false;
		break;
	}
}

bool PatientDataCenter::readCurrentGameSummaryData(CentPatient::SummaryData& summaryData, DataHandleMethod method)//TODO
{
	Q_UNUSED(summaryData);

	switch(method) //TODO: add a map with DataHandleMethod to read method/object mapping
	{
		case FILE_SYSTEM:
			return false; //TODO: add a method for reading sumarry data
		break;

		default:
			qCritical() << __FUNCTION__ << "Unknown save method for patient summary data";
			return false;
		break;
	}
}

//static
QString PatientDataCenter::getDataParentDir()
{
	QSettings globalSettings(QSettings::SystemScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);
	QString dataParentDir = globalSettings.value(CENT::Settings::PARENT_DATA_DIR).toString();
	if(dataParentDir.isEmpty())
	{
		// Read my documents from registry as fallback
		QSettings settings(PatientDataCenterConstants::MY_DOCUMENTS_REGISTRY_KEY, QSettings::NativeFormat);
		dataParentDir = settings.value(PatientDataCenterConstants::MY_DOCUMENTS_REGISTRY_VALUE).toString();
	}
	return dataParentDir;
}

void PatientDataCenter::setDataParentDir()
{
	m_dataParentDir = getDataParentDir();
}

bool PatientDataCenter::createBaseDirectoryStructure()
{
	qWarning() << "Creating base directory structures";

	QString currentDir = m_dataParentDir;
	QStringList baseDirectoryStruct;
	baseDirectoryStruct << PatientDataCenterConstants::BASE_DIR
		                << PatientDataCenterConstants::PATIENTS_DIR;

	return CentFileHelpers::createPath(currentDir, baseDirectoryStruct);
}

QString PatientDataCenter::dirForPatient(const QString& patientName)
{
	QDir dir(QString("%1%2%3").arg(m_dataParentDir)
		.arg(QDir::separator() + PatientDataCenterConstants::BASE_DIR)
		.arg(QDir::separator() + PatientDataCenterConstants::PATIENTS_DIR));

	QStringList entryList = dir.entryList(QDir::AllDirs);

	foreach (const QString& dir, entryList)
	{
		if (patientName == m_cipher.decrypt(dir))
		{
			return dir;
		}
	}

	return m_cipher.encrypt(patientName);
}

bool PatientDataCenter::writePatientToFileSystem(const CentPatient::Data& data)
{
	QString currentDir = m_dataParentDir;

	// Create paths
	QString dateString = data.m_date.toString(PatientDataCenterConstants::DATE_FORMATTER);

	QStringList patientSessionDirList;
	patientSessionDirList << PatientDataCenterConstants::BASE_DIR
		                  << PatientDataCenterConstants::PATIENTS_DIR
		                  << PatientDataCenterConstants::PATIENT_DIRECTORY_FORMATTER.arg(dirForPatient(data.m_userName));
//		                  << PatientDataCenterConstants::PATIENT_SESSION_DIRECTORY_FORMATTER.arg(dateString);

	if(! CentFileHelpers::createPath(currentDir, patientSessionDirList))
	{
		return false;
	}

	m_currentPatientDir = currentDir;
	emit currentPatientPathChanged(m_currentPatientDir);

	patientSessionDirList.clear();
	patientSessionDirList << PatientDataCenterConstants::PATIENT_SESSION_DIRECTORY_FORMATTER.arg(dateString);
	if(! CentFileHelpers::createPath(currentDir, patientSessionDirList))
	{
		return false;
	}

	m_currentSessionDir = currentDir;
	m_currentSessionDate = data.m_date;
	emit currentSessionPathChanged(m_currentSessionDir);

	QString patiensConditionFilePath = currentDir
		                             + QDir::separator()
		                             + PatientDataCenterConstants::PATIENT_DATA_FILENAME;

	PatientDataUnit conditionData(patiensConditionFilePath);
	conditionData.setValue(PatientDataCenterConstants::HOURS_SLEPT_SETTING, data.m_hoursSlept);
	conditionData.setValue(PatientDataCenterConstants::HOURS_SINCE_LAST_SLEEP_SETTING, data.m_hoursSinceLastSleep);
	conditionData.setValue(PatientDataCenterConstants::MOTIVATION_SETTING, data.m_motivation);
	conditionData.setValue(PatientDataCenterConstants::MOOD_SETTING, data.m_mood);
	conditionData.setValue(PatientDataCenterConstants::EXITEMENT_SETTING, data.m_excitement);
	conditionData.setValue(PatientDataCenterConstants::SESSION_DATE_SETTING, dateString);
	conditionData.sync();

	if(conditionData.status() != PatientDataUnit::NoError)
	{
		qCritical() << __FUNCTION__ << "Could not create patient condition file : " << patiensConditionFilePath;
		return false;
	}
	backupPatientsFolder();
	return true;
}

bool PatientDataCenter::writeGameDataToFileSystem(const CentPatient::GameData& gameData)
{
	if(! m_currentUserSelected)
	{
		qCritical() << __FUNCTION__ << "Current user not set.";
		return false;
	}

	// Create directories
	QString currentDir = m_currentSessionDir;

	QStringList currentGameDirList;
	QString dateString = gameData.m_gameDate.toString(PatientDataCenterConstants::DATE_FORMATTER);

	currentGameDirList << PatientDataCenterConstants::GAMES_DIR
		               << PatientDataCenterConstants::ONE_GAME_DIR.arg(gameData.m_gameName).arg(dateString);

	if(! CentFileHelpers::createPath(currentDir, currentGameDirList))
	{
		return false;
	}

	m_currentSessionGameDir = currentDir;
	emit currentGamePathChanged(m_currentSessionGameDir);

	// Open parameters file
	QString gameParametersFilePath = currentDir
		                           + QDir::separator()
		                           + PatientDataCenterConstants::ONE_GAME_PARAMETERS_FILENAME;

	PatientDataUnit gameParametersDataUnit(gameParametersFilePath);

	gameParametersDataUnit.setValue(PatientDataCenterConstants::GAME_BETA_COEFF_USED, QString::number(m_currentGameBetaCoeff, 'f', 2 ));
	gameParametersDataUnit.setValue(PatientDataCenterConstants::GAME_THETA_COEFF_USED, QString::number(m_currentGameThetaCoeff, 'f', 2 ));
	gameParametersDataUnit.sync();

	if(gameParametersDataUnit.status() != QSettings::NoError)
	{
		qCritical() << __FUNCTION__ << "Could not create patient game parameters file: " << gameParametersFilePath;
		return false;
	}

	m_currentGame = gameData;
	m_currentGameSelected = true;

	backupPatientsFolder();
	return true;
}

bool PatientDataCenter::saveCoefficients(const double& beta, const double& theta)
{
	m_currentGameBetaCoeff = beta;
	m_currentGameThetaCoeff = theta;
	bool b = writeOVBoxSettingFile(CENT::DirectoryId::DirectoryCurrentSession, CENT::FileNameId::FileNameBetaCoeff, QStringList(QString::number(beta, 'f', 2 )));
	return b && writeOVBoxSettingFile(CENT::DirectoryId::DirectoryCurrentSession, CENT::FileNameId::FileNameThetaCoeff, QStringList(QString::number(theta, 'f', 2 )));
}

bool PatientDataCenter::writeOVBoxSettingFile(CENT::DirectoryId::Id dirId, CENT::FileNameId::Id fileId, const QStringList& settingsList)
{
	QString path = *m_directories.value(dirId);
	if(path.isEmpty())
	{
		qCritical() << __FUNCTION__ << "Missing directory path.";
		return false;
	}

	path += QDir::separator() + *m_filenames.value(fileId);
	return CentFileHelpers::writeConfigFile(path, settingsList);
}

bool PatientDataCenter::saveEvaluationData(const CentPatient::EvaluationData& data)
{
	if(! m_currentUserSelected)
	{
		qCritical() << __FUNCTION__ << "Current user not selected.";
		return false;
	}

	if(m_currentSessionDir.isEmpty())
	{
		qCritical() << __FUNCTION__ << "Current session not selected.";
		return false;
	}

	if(data.m_performance != CentPatient::INVALID && data.m_stress != CentPatient::INVALID)
	{
		// Open one game summary file
		QString evaluationFilePath = m_currentSessionDir
			                        + QDir::separator()
			                        + PatientDataCenterConstants::SESSION_SUMMARY_FILENAME;

		//TODO: refactor this whole file to use PatientDataUnit
		PatientDataUnit evaluationDataFile(evaluationFilePath);

		evaluationDataFile.setValue(PatientDataCenterConstants::SESSION_EFFORT_SETTING, data.m_performance);
		evaluationDataFile.setValue(PatientDataCenterConstants::SESSION_FRUSTRATION_SETTING, data.m_stress);
		evaluationDataFile.sync();

		if(evaluationDataFile.status() != QSettings::NoError)
		{
			qCritical() << __FUNCTION__ << "Could not create patient session summary file: " << evaluationFilePath;
			return false;
		}
		backupPatientsFolder();

		return true;
	}
	else
	{
		qCritical() << __FUNCTION__ << "Game summary input is invalid.";
		return false;
	}
}

void PatientDataCenter::triggerParseBetaTheta()
{
	QStringList list;
	QString str;
	double beta, theta;
	bool ok;

	if(!CentFileHelpers::parseConfigFile(m_currentSessionDir + QDir::separator() + PatientDataCenterConstants::BETA_LEVEL_FILENAME, list))
	{
		return;	//error is logged in the method above
	}
	str = list[0];
	beta = str.toDouble(&ok);
	if(!ok)
	{
		qCritical() << __FUNCTION__ << "Setting read is in incorrect format.";
		return;
	}

	if(!CentFileHelpers::parseConfigFile(m_currentSessionDir + QDir::separator() + PatientDataCenterConstants::THETA_LEVEL_FILENAME, list))
	{
		return;	//error is logged in the method above
	}
	str = list[0];
	theta = str.toDouble(&ok);
	if(!ok)
	{
		qCritical() << __FUNCTION__ << "Setting read is in incorrect format.";
		return;
	}

	emit betaThetaParsed(beta, theta);
}

void PatientDataCenter::triggerReloadGamesSummary()
{
	QStringList list;
	if(!CentFileHelpers::findFiles(m_currentSessionDir, PatientDataCenterConstants::ONE_GAME_SUMMARY_FILENAME, list))
	{
		//error already logged
		return;
	}

	qSort(list.begin(), list.end(), pathByFileDateLessThan);

	QList<QPair<QString, double> > scores;
	QPair<QString, double> gameScore;
	//parse files
	foreach(const QString& filename, list)
	{
		if(!getGameScoreFromSummary(filename, gameScore))
		{
			qWarning() << "Score could not be read from file: " << filename;
		}
		else
		{
			scores.append(gameScore);
		}
	}
	emit gameScoresLoaded(scores);
}

void PatientDataCenter::triggerReloadSessionsSummary()
{
	QStringList list;
	if(!CentFileHelpers::findFiles(m_currentPatientDir, PatientDataCenterConstants::SESSION_SCORE_FILENAME, list))
	{
		//error already logged
		return;
	}

	//the list should be sorted correctly by folder names that are named by date

	QList<QPair<QString, double> > scores;
	QPair<QString, double> sessionScore;
	//parse files
	foreach(const QString& filename, list)
	{
		if(!getSessionScoreFromSummary(filename, sessionScore))
		{
			qWarning() << "Score could not be read from file: " << filename;
		}
		else
		{
			scores.append(sessionScore);
		}
	}
	emit sessionResultsLoaded(scores);
}

void PatientDataCenter::resetSession()
{
	m_currentUser = CentPatient::Data();
	m_currentGame = CentPatient::GameData();

	m_currentPatientDir.clear();
	m_currentSessionDir.clear();
	m_currentSessionGameDir.clear();

	m_currentUserSelected = false;
	m_currentGameSelected = false;
}

bool PatientDataCenter::writeSummaryDataToFileSystem(const CentPatient::SummaryData& summaryData)
{
	if(! m_currentUserSelected)
	{
		qCritical() << __FUNCTION__ << "Current user not selected.";
		return false;
	}

	if(! m_currentGameSelected)
	{
		qCritical() << __FUNCTION__ << "Current game not selected.";
		return false;
	}

	if(summaryData.m_gameScore != CentPatient::INVALID)
	{
		// Open one game summary file
		QString gameSummaryFilePath = m_currentSessionGameDir
			                        + QDir::separator()
			                        + PatientDataCenterConstants::ONE_GAME_SUMMARY_FILENAME;

		//TODO: refactor this whole file to use PatientDataUnit
		PatientDataUnit gameSummaryData(gameSummaryFilePath);

		gameSummaryData.setValue(PatientDataCenterConstants::GAME_NAME_SETTING, m_currentGame.m_gameName);
		gameSummaryData.setValue(PatientDataCenterConstants::GAME_SCORE_SETTING, summaryData.m_gameScore);
		gameSummaryData.sync();

		if(gameSummaryData.status() != QSettings::NoError)
		{
			qCritical() << __FUNCTION__ << "Could not create patient game summary file: " << gameSummaryFilePath;
			return false;
		}
		backupPatientsFolder();

		triggerReloadGamesSummary();
		return true;
	}
	else
	{
		qCritical() << __FUNCTION__ << "Game summary input is invalid.";
		return false;
	}
}


bool PatientDataCenter::writeSessionScore(const QList<SCORE_TYPE>& data)
{
	if(! m_currentUserSelected)
	{
		qCritical() << __FUNCTION__ << "Current user not selected.";
		return false;
	}

	// Session score file name
	QString sessionScoreFilePath = m_currentSessionDir
		                        + QDir::separator()
		                        + PatientDataCenterConstants::SESSION_SCORE_FILENAME;

	double scoreTmp = 0;
	int count = 0;
	
	foreach(const SCORE_TYPE& score, data)
	{
		scoreTmp += score.second;
		count++;
	}
	scoreTmp /= count;

	PatientDataUnit sessionScoreData(sessionScoreFilePath);
	sessionScoreData.setValue(PatientDataCenterConstants::SESSION_SCORE_SETTING, scoreTmp);
	sessionScoreData.setValue(PatientDataCenterConstants::SESSION_DATE_SETTING, m_currentSessionDate.toString(DATE_BAR_CHART_FORMATTER));

	if(sessionScoreData.status() != PatientDataUnit::NoError)
	{
		qCritical() << __FUNCTION__ << "Failed to save file: " << sessionScoreFilePath;
		return false;
	}
	backupPatientsFolder();
	return true;
}

bool PatientDataCenter::getGameScoreFromSummary(const QString& filename, SCORE_TYPE& data)
{
	PatientDataUnit settingFile(filename);
	data.first = settingFile.value(PatientDataCenterConstants::GAME_NAME_SETTING).toString();
	bool b = true;
	data.second = settingFile.value(PatientDataCenterConstants::GAME_SCORE_SETTING, 0.0).toDouble(&b);

	if(settingFile.status() != QSettings::NoError || data.first.isEmpty() || !b)
	{
		qCritical() << __FUNCTION__ << "Failed to open and parse file: " << filename;
		return false;
	}
	return true;
}

bool PatientDataCenter::getSessionScoreFromSummary(const QString& filename, SCORE_TYPE& data)
{
	PatientDataUnit settingFile(filename);
	data.first = settingFile.value(PatientDataCenterConstants::SESSION_DATE_SETTING).toString();
	bool b = true;
	data.second = settingFile.value(PatientDataCenterConstants::SESSION_SCORE_SETTING, 0.0).toDouble(&b);

	if(settingFile.status() != QSettings::NoError || data.first.isEmpty() || !b)
	{
		qCritical() << __FUNCTION__ << "Failed to open and parse file: " << filename;
		return false;
	}
	return true;
}

bool PatientDataCenter::backupPatientsFolder()
{
	return m_backupManager->backupPatientsFolder();
}
