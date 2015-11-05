#ifndef BASE_CENT_PAGE_CONTROLLER_H
#define BASE_CENT_PAGE_CONTROLLER_H

#include <QWidget>
#include <QVariantList>

#include "CentActions.h"
#include "StateEnums.h"

class CentMainWindow;
class ICentPage;
class IResultReceiver;

class BaseCentPageController: public QObject
{
	Q_OBJECT
public:
	explicit BaseCentPageController(CentMainWindow& resercherWindow,CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~BaseCentPageController();

	virtual bool activatePage();
	virtual bool deactivatePage();

	bool registerPage();

public slots:
	void loadHeaders();

signals:
	void goToState(CentState state);
	void error(const QString& errStr);
	void invokeAction(CENT::ActionType actionType, const QVariantList& params, IResultReceiver* invoker, CentState stateOnSuccess);
	void invokeAction(CENT::ActionType actionType, const QVariantList& params, IResultReceiver* invoker); // stateOnSuccess = ControlStateUndefined
	void invokeAction(CENT::ActionType actionType, const QVariantList& params); // invoker = 0, stateOnSuccess = ControlStateUndefined

protected:
	virtual ICentPage* createResearcherPage() = 0;
	virtual ICentPage* createPatientPage() = 0;

private:
	ICentPage* getResearcherPage();
	ICentPage* getPatientPage();

	bool activatePage(CentMainWindow& mainWindow, ICentPage* page);
	void loadHeaders(CentMainWindow& mainWindow, ICentPage* page);

private:
	CentMainWindow& m_researcherWindow;
	CentMainWindow& m_patientWindow;

	ICentPage*      m_patientPage;
	ICentPage*      m_researcherPage;
};

#endif // BASE_CENT_PAGE_CONTROLLER_H
