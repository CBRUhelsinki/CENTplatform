#ifndef CENT_ACTION_HANDLER_H
#define CENT_ACTION_HANDLER_H

#include <QMap>
#include <QObject>
#include <QVariantList>

#include "CentActions.h"
#include "OVAppsRunner.h"
#include "StateEnums.h"
#include "CentErrors.h"

class IResultReceiver;
class CentStateManager;
class CentGameManager;
class PatientDataCenter;
class VRPNServerThread;
class SessionNotesLauncher;

class CentActionHandler : public QObject
{
	Q_OBJECT
public:
	explicit CentActionHandler(CentStateManager& stateManager,QObject* parent = 0);
	virtual ~CentActionHandler();

public:
	void setOVAppsRunner(OVAppsRunner* appsRunner);
	void setGameManager(CentGameManager* gameManager);
	void setVrpnServer(VRPNServerThread* vrpnServer);
	void setPatientDataCenter(PatientDataCenter* dataCenter);
	void setSessionNotesLauncher(SessionNotesLauncher* sessionNotesLauncher);

public slots:
	void executeAction(CENT::ActionType actionType, const QVariantList& params, IResultReceiver* invoker = 0, CentState stateOnSuccess = ControlStateUndefined);
	void executionResult(int error);

private:
	int runOVDesigner(const QVariantList& params);
	int stopOVDesigner(const QVariantList& params);
	int setCurrentGame(const QVariantList& params);
	int setStimulationButton(const QVariantList& params);
	int parseBetaThetaLevelValues(const QVariantList& params);
	int saveCoeffValues(const QVariantList& params);
	int saveEvaluation(const QVariantList& params);
	int useDefaultBaseline(const QVariantList& params);
	int showSettings(const QVariantList& params);
	int endSession(const QVariantList& params);

private:
	typedef int (CentActionHandler::*ExecutionMethodPtr)(const QVariantList&);
	struct ActionData
	{
		ActionData()
			: m_paramCount(-1), m_executionMethodPtr(NULL)
		{
		}
		ActionData(int paramCount, ExecutionMethodPtr executionMethodPtr)
		{
			m_paramCount = paramCount;
			m_executionMethodPtr = executionMethodPtr;
		}
		int m_paramCount;
		ExecutionMethodPtr m_executionMethodPtr;
	};

	QMap<CENT::ActionType, ActionData> m_actionsMap;
	OVAppsRunner*         m_ovAppsRunner;
	CentStateManager&     m_stateManager;
	CentState             m_stateOnSuccess;
	IResultReceiver*      m_currentReceiver;
	CentGameManager*      m_gameManager;
	VRPNServerThread*     m_vrpnServer;
	PatientDataCenter*    m_patientDataCenter;
	SessionNotesLauncher* m_sessionNotesLauncher;
};
#endif // CENT_ACTION_HANDLER_H
