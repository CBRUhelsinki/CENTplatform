#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

#include <QObject>
#include <QPointer>

class QProcess;

class BackupManager : public QObject
{
	Q_OBJECT
public:
	BackupManager(const QString& src, QObject* parent = 0);
	~BackupManager();

	void setDestSubdir(const QString& sub);

public slots:
	bool backupPatientsFolder();

private:
	Q_DISABLE_COPY(BackupManager)

	QString m_sourcePath;
	QString m_destSubdir;

	//Used to start backup in background
	QPointer<QProcess> m_backupProcess;

};
#endif // BACKUP_MANAGER_H