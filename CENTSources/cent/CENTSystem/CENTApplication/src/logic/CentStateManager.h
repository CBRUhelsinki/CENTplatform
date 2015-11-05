#ifndef CENT_STATE_MANAGER_H
#define CENT_STATE_MANAGER_H

#include <QObject>
#include "LogicUiFusion.h"
#include "StateEnums.h"

class CentStateManager: public QObject
{
	friend class CentStateManager_testable;

	Q_OBJECT
public:
	explicit CentStateManager(LogicUiFusion<CentState, CentUiState>& logicUiFusion, QObject* parent = 0);
	virtual ~CentStateManager();

public slots:
	void onNext(); //TODO: move to private
	void onBack(); //TODO: move to private
	void onClose(); //TODO: remove??

	void goToState(CentState state);

private:
	void setCurrentState(CentState state);

private:
	CentState m_currentState;

	LogicUiFusion<CentState, CentUiState>& m_logicUiFusion;
};
#endif // CENT_STATE_MANAGER_H
