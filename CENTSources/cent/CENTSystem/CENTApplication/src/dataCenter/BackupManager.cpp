#include "BackupManager.h"

#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QTimer>
#include <QSettings>

#include "CentConstants.h"
#include "CentFileHelpers.h"

namespace
{
	const int RETRY_MSEC         = 3000; // 3s

	const QString ROBOCOPY_SUBDIR_PARAM = "/S";
	const QString ROBOCOPY_LOG_PARAM    = "/LOG:robocopy.log";
	const QString ROBOCOPY_COMMAND      = "robocopy";
}

BackupManager::BackupManager(const QString& src, QObject* parent)
	: QObject(parent)
	, m_sourcePath(src)
	, m_backupProcess(new QProcess)
{

}

BackupManager::~BackupManager()
{
	delete m_backupProcess;
}

void BackupManager::setDestSubdir(const QString& sub)
{
	m_destSubdir = sub;
}

bool BackupManager::backupPatientsFolder()
{
	if(m_backupProcess->state() == QProcess::Running)
	{
		//we are still running, lets shedule a retry
		QTimer::singleShot(RETRY_MSEC, this, SLOT(backupPatientsFolder()));
	}

	if(m_backupProcess->exitCode() >= 4)	// reference: http://ss64.com/nt/robocopy-exit.html
	{
		qCritical() << "Failed to backup Patients folder, error string: " << m_backupProcess->readAllStandardError();
		return false;
	}

	QSettings userSettings(QSettings::UserScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);
	QString dest = userSettings.value(CENT::Settings::DATA_REPLICATION_PATH).toString();
	if(! CentFileHelpers::verifyWriteRightsInFolder(dest))
	{
		return false; // error already logged
	}

	if(!m_destSubdir.isEmpty())
	{
		dest += QDir::separator() + m_destSubdir;
	}

	QStringList params;
	params << m_sourcePath << dest << ROBOCOPY_SUBDIR_PARAM << ROBOCOPY_LOG_PARAM;

	m_backupProcess->start(ROBOCOPY_COMMAND, params);
	return true;
}
