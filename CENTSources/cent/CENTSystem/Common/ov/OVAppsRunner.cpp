#include "OVAppsRunner.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

#include "CentConstants.h"
#include "CentFileHelpers.h"
#include "Connect.h"
#include "PatientDataCenter.h"
#include "VRPNServerThread.h"

namespace
{
	const QString SCENARIOS_SUB_PATH = "/scenarios/";
	const QString ENV_PATH_VAR = "PATH";
	const QString SEPARATOR = QDir::separator();

	const QString ENV_CURRENT_SESSION = "CENT_CURRENT_SESSION";
	const QString ENV_CURRENT_GAME    = "CENT_CURRENT_GAME";
	const QString ENV_CURRENT_PATIENT = "CENT_CURRENT_PATIENT";
	const QString ENV_SCENARIOS_HOME  = "CENT_SCENARIOS_HOME";
	const QString ENV_OPENVIBE_HOME   = "OPENVIBE_HOME";

	const char* const ACQ_EXIT_INPUT = "c\n";

	const QString EEG_SCENARIO_PREFIX      = "cent_monitoring_and_noise";
	const QString BASELINE_SCENARIO_PREFIX = "cent_baseline";
	const QString BASELINE_PROCESS_PREFIX  = "cent_generate_configuration";
	const QString GAME_SCENARIO_PREFIX     = "cent_game";
	const QString SCENARIO_FILE_EXTENSION  = ".xml";

	const int APP_EXIT_TIMEOUT_MS = 4000;
	const int APP_START_TIMEOUT_MS = 3000;

}

OVAppsRunner::OVAppsRunner(QSettings& settings, VRPNServerThread& vrpnServer, PatientDataCenter& dataCenter, QObject* parent)
	: QObject(parent)
	, m_acqProcess(0)
	, m_ovProcess(0)
	, m_settings(settings)
	, m_vrpnServer(vrpnServer)
	, m_dataCenter(dataCenter)
	, m_currentScenarioGroup("Cent default")
	, m_initialized(false)
	, m_scenarioGroups()
{
	m_scenarioMap.insert(OPENVIBE::EEGWithNoise,        EEG_SCENARIO_PREFIX      + SCENARIO_FILE_EXTENSION);
	m_scenarioMap.insert(OPENVIBE::BaseLine,            BASELINE_SCENARIO_PREFIX + SCENARIO_FILE_EXTENSION);
	m_scenarioMap.insert(OPENVIBE::BaseLinePostProcess, BASELINE_PROCESS_PREFIX  + SCENARIO_FILE_EXTENSION);
	m_scenarioMap.insert(OPENVIBE::Game,                GAME_SCENARIO_PREFIX     + SCENARIO_FILE_EXTENSION);

	initialize();
}

OVAppsRunner::~OVAppsRunner()
{
	stopACQ();
	stopOV();
}

void OVAppsRunner::stopACQ()
{
	if(m_acqProcess)
	{
		m_acqProcess->write(ACQ_EXIT_INPUT);
		if(!m_acqProcess->waitForFinished(APP_EXIT_TIMEOUT_MS))
		{
			m_acqProcess->kill();
		}
		delete m_acqProcess;
		m_acqProcess = 0;
	}
}

void OVAppsRunner::stopOV()
{
	if(m_ovProcess)
	{
		// should activate OVTK_StimulationId_ExperimentStop stimulation in scenario
		m_vrpnServer.setButtonState(CENT::Stimulation::SCENARIO_CONTROL, CENT::Stimulation::ON);

		if(m_ovProcess->state() != QProcess::NotRunning && !m_ovProcess->waitForFinished(APP_EXIT_TIMEOUT_MS))
		{
			m_ovProcess->kill();
		}
		delete m_ovProcess;
		m_ovProcess = 0;
	}
}

void OVAppsRunner::startOV(OPENVIBE::DesignerScenarioType mode)
{
	QString scenarioName = m_scenarioMap.value(mode);
	if(scenarioName.isEmpty())
	{
		emit startResult(RunAppWrongParam);
		return;
	}
	QString scenarioPath = m_myPath + QDir::separator() + 
	                       SCENARIOS_SUB_PATH + QDir::separator() + m_capDrivers.path + QDir::separator() +
	                       m_currentScenarioGroup + QDir::separator() + scenarioName;

	// should activate OVTK_StimulationId_ExperimentStart stimulation in scenario
	m_vrpnServer.setButtonState(CENT::Stimulation::SCENARIO_CONTROL, CENT::Stimulation::OFF);

	startOV(scenarioPath);
}

void OVAppsRunner::startACQ()
{
	QFile ovAcqBin(m_myPath + QDir::separator() + OPENVIBE::ACQ_SERVER_EXE_NAME);
	QStringList params;
	params << m_capDrivers.cmd.split(' ', QString::SkipEmptyParts);
	RunAppError error = startProcess(&m_acqProcess, ovAcqBin, params);
	emit startResult(error);
}

void OVAppsRunner::startOV(const QString& scenarioPath)
{
	QFile ovDesignerBin(m_myPath + QDir::separator() + OPENVIBE::OPENVIBE_EXE_NAME);
	QStringList ovParams;
	ovParams << OPENVIBE::RUN_PARAMS::NO_GUI << OPENVIBE::RUN_PARAMS::NO_MANAGEMENT
	         << OPENVIBE::RUN_PARAMS::PLAY_SCENARIO << scenarioPath;
	RunAppError error = startProcess(&m_ovProcess, ovDesignerBin, ovParams);
	emit startResult(error);
}

void OVAppsRunner::triggerReloadScenarios()
{
	QStringList scenarioGroups;
	if(CentFileHelpers::findFoldersWithFiles(m_myPath + QDir::separator() + SCENARIOS_SUB_PATH + QDir::separator() +
		m_capDrivers.path, m_scenarioMap.values(), scenarioGroups, 1))
	{
		m_scenarioGroups.clear();
		foreach(const QString& name, scenarioGroups)
		{
			m_scenarioGroups.insert(name.right(name.length() - name.lastIndexOf(QDir::separator()) - 1), name);
		}
	}
	emit scenarioGroupsLoaded(m_scenarioGroups.keys());

	QString newGroupName(m_currentScenarioGroup);
	if(!m_scenarioGroups.keys().contains(m_currentScenarioGroup))
	{
		if(m_scenarioGroups.count() > 0)
		{
			newGroupName = m_scenarioGroups.keys().first();
		}
		else
		{
			newGroupName = QString();
		}
	}
	setScenarioGroup(newGroupName);
}

// If we want to specify a standalone scenario group in the future we can add setScenarioGroup(const QDir&)
void OVAppsRunner::setScenarioGroup(const QString& name)
{
	QString path = m_scenarioGroups.value(name);
	if(path.isEmpty())
	{
		qWarning() << "Unknown scenario group: " << name;
	}
	else
	{
		m_currentScenarioGroup = name;
	}
	emit currentScenarioGroupChanged(m_currentScenarioGroup);
}

OVAppsRunner::RunAppError OVAppsRunner::startProcess(QProcess** mProc, const QFile& exeFile, const QStringList& params)
{
	if(!m_initialized)
	{
		qCritical() << __FUNCTION__ << "Not initialized, not going to start: " << exeFile.fileName().toAscii();
		return RunAppNotInitialized;
	}

	if(*mProc) // mProc should be NULL to continue
	{
		qWarning() << __FUNCTION__ << "Already started, not going to start: " << exeFile.fileName().toAscii();
		return RunAppAlreadyStarted;
	}

	QDir workingDir = QDir::current();
	QDir::setCurrent(m_ovBin.path());

	RunAppError error = RunAppSuccess;
	if(!exeFile.exists())
	{
		qCritical() << exeFile.fileName() << " file doesn't exist.";
		error = RunAppNotFound;
	}

	if(error == RunAppSuccess)
	{
		m_runEnv.insert(ENV_CURRENT_SESSION, m_dataCenter.currentSessionPath());
		m_runEnv.insert(ENV_CURRENT_GAME,    m_dataCenter.currentGamePath());
		m_runEnv.insert(ENV_CURRENT_PATIENT, m_dataCenter.currentPatientPath());
		m_runEnv.insert(ENV_SCENARIOS_HOME,  m_myPath + SEPARATOR + SCENARIOS_SUB_PATH + SEPARATOR + m_capDrivers.path + SEPARATOR + m_currentScenarioGroup);

		QProcess* proc = *mProc = new QProcess();
		proc->setProcessChannelMode(QProcess::MergedChannels);
		proc->setProcessEnvironment(m_runEnv);
		CENT::connect(proc, SIGNAL(readyRead()), this, SLOT(gotConsoleOutput()));

		proc->start(exeFile.fileName(), params);

		proc->waitForStarted(APP_START_TIMEOUT_MS);
		if(proc->state() == QProcess::Running)
		{
			error = RunAppSuccess;
		}
		else
		{
			error = RunAppUnknownError;
		}
	}
	QDir::setCurrent(workingDir.path());
	return error;
}

void OVAppsRunner::initialize()
{
	QString ovPath = m_settings.value(CENT::Settings::OPENVIBE_INSTALLATION_FOLDER).toString();
	QDir ovDir(ovPath);
	if(!ovDir.exists())
	{
		qCritical() << __FUNCTION__ << "OpenVibe installation dir doesn't exist. Tried: " << ovPath;
		return;
	}

	QDir ovDeps(ovDir);
	if(!ovDeps.cd(OPENVIBE::DEPS::DIRECTORY_PATH))
	{
		qCritical() << __FUNCTION__ << "OpenVibe dependencies dir doesn't exist. Tried: " << OPENVIBE::DEPS::DIRECTORY_PATH;
		return;
	}

	QDir ovBin(ovDir);
	if(!ovBin.cd(OPENVIBE::BIN_DIRECTORY))
	{
		if(!ovBin.cd(OPENVIBE::ALTERNATE_BIN_DIRECTORY))
		{
			qCritical() << __FUNCTION__ << "OpenVibe bin dir doesn't exist. Tried: " << OPENVIBE::ALTERNATE_BIN_DIRECTORY;
			return;
		}
	}
	QSettings userSettings(QSettings::UserScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);
	CentData::CapDrivers capDrivers = static_cast<CentData::CapDrivers>(userSettings.value(CENT::Settings::CAP_DRIVER_TYPE
		, CentData::UnknowCapDriver).toInt());
	if(capDrivers == CentData::UnknowCapDriver)
	{
		qCritical() << __FUNCTION__ << "Cap Drivers not selected.";
		return;
	}
	
	m_capDrivers.type = capDrivers;
	m_capDrivers.cmd  =  userSettings.value(CENT::Settings::CAP_DRIVER_CMD).toString();
	switch(capDrivers)
	{
		case CentData::CapDriver4thChannels:
			m_capDrivers.path = CENT::DirectoryName::CAP_DRIVER_4CHANNELS_FOLDER_NAME;
			break;
		case CentData::CapDriver8thChannels:
			m_capDrivers.path = CENT::DirectoryName::CAP_DRIVER_8CHANNELS_FOLDER_NAME;
			break;
		default:
			break;
	}

	m_ovDeps = ovDeps;
	m_ovBin = ovBin;
	m_ovPath = ovPath;

	m_myPath = QCoreApplication::applicationDirPath();
	m_initialized = setupRunEnv();
}

bool OVAppsRunner::setupRunEnv()
{
	m_runEnv = QProcessEnvironment::systemEnvironment();
	m_runEnv.insert(ENV_PATH_VAR, 
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_LIBEXPAT +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_GTK +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_OGRE_DEBUG +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_OGRE_RELEASE +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_ITPP +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_LUA +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_OPENAL +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_FREEALUT +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_VORBIS_DEBUG +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_VORBIS_RELEASE +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_OGG_DEBUG +
		m_ovDeps.path() + OPENVIBE::DEPS::SUBPATH_OGG_RELEASE +
		m_ovBin.path() + ";" + m_runEnv.value(ENV_PATH_VAR) + ";");

	m_runEnv.insert(ENV_OPENVIBE_HOME, m_ovPath);
	return true;
}

void OVAppsRunner::gotConsoleOutput()
{
	QProcess* proc = sender() == m_acqProcess ? m_acqProcess : m_ovProcess; // faster than qobject_cast
	const QString title = proc == m_acqProcess ? tr("ACQ Server") : tr("OV Designer");
	if (proc)
	{
		QByteArray data = proc->readAllStandardOutput();
		emit consoleOutput(data, title);
	}
}
