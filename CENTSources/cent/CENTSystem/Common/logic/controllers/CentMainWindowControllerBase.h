#ifndef CENT_MAIN_WINDOW_CONTROLLER_BASE_H
#define CENT_MAIN_WINDOW_CONTROLLER_BASE_H

#include <QObject>

class CentMainWindow;

class CentMainWindowControllerBase : public QObject
{
	Q_OBJECT

public:
	explicit CentMainWindowControllerBase(CentMainWindow& researcherMainWindow, CentMainWindow& patientMainWindow, QObject* parent = 0);
	virtual ~CentMainWindowControllerBase();

signals:
	void next();
	void back();
	void close();

public slots:
	void closeMainWindow();
	virtual void onUiStateChanged(int state) = 0;

protected:
	void createConnections();

private:
	void createConnections(CentMainWindow& window);

protected:
	CentMainWindow& m_researcherMainWindow;
	CentMainWindow& m_patientMainWindow;
};

#endif
