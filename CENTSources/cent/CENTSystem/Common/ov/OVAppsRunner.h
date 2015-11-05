#ifndef OV_APPS_RUNNER_H
#define OV_APPS_RUNNER_H

#include <QDir>
#include <QMap>
#include <QObject>
#include <QProcess>

#include "OpenVibeConstants.h"
#include "CentDataTypes.h"

class QSettings;

class PatientDataCenter;
class VRPNServerThread;

class OVAppsRunner : public QObject
{
	Q_OBJECT
public:
	explicit OVAppsRunner(QSettings& settings, VRPNServerThread& vrpnServer, PatientDataCenter& dataCenter, QObject* parent = 0);
	virtual ~OVAppsRunner();

	enum RunAppError
	{
		RunAppSuccess = 0,
		RunAppNotFound,
		RunAppNotInitialized,
		RunAppAlreadyStarted,
		RunAppWrongParam,
		RunAppUnknownError
	};

public slots:
	void stopACQ();
	void stopOV();
	void startOV(OPENVIBE::DesignerScenarioType mode);
	void startACQ();

	void triggerReloadScenarios();
	void setScenarioGroup(const QString& name);

private slots:
	void gotConsoleOutput();

signals:
	void startResult(int error);
	void consoleOutput(QString data, QString processTitle);
	void scenarioGroupsLoaded(QStringList scenarios);
	void currentScenarioGroupChanged(QString);

private:
	void initialize();
	bool setupRunEnv();
	void startOV(const QString& scenarioName);
	RunAppError startProcess(QProcess** mProc, const QFile& exeFile, const QStringList& params = QStringList());

private:
	struct CapDriverData
	{
		CentData::CapDrivers type;
		QString              path;
		QString              cmd;
	};
	VRPNServerThread&  m_vrpnServer;
	PatientDataCenter& m_dataCenter;

	QSettings&    m_settings;
	QProcess*     m_acqProcess;
	QProcess*     m_ovProcess;
	QDir          m_ovDeps;
	QDir          m_ovBin;
	QString       m_myPath;
	QString       m_ovPath;
	bool          m_initialized;
	QString       m_currentScenarioGroup;
	CapDriverData m_capDrivers;

	QMap<QString, QString>                        m_scenarioGroups;
	QMap<OPENVIBE::DesignerScenarioType, QString> m_scenarioMap;
	
	QProcessEnvironment m_runEnv;
};

#endif // OV_APPS_RUNNER_H
