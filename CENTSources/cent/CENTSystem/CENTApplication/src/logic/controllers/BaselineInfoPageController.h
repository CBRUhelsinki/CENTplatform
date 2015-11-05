#ifndef BASELINE_INFO_PAGE_CONTROLLER_H
#define BASELINE_INFO_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "IResultReceiver.h"
#include "PowerSpectrumData.h"
#include "StateEnums.h"

class BaselineInfoPage;
class CentMainWindow;

class BaselineInfoPageController : public BaseCentPageController , public IResultReceiver
{
	Q_OBJECT
public:
	BaselineInfoPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~BaselineInfoPageController();

	virtual bool activatePage();
	virtual void result(int err); // from IResultReceiver

signals:
	void gameTimeoutChanged(int minutes);

public slots:
	void setCurrentSessionPath(const QString& path);
	void setBetaThetaLevels(double beta, double theta);
	void resetSession();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private slots:
	void onReRecord();
	void onNext();

private:
	QString           m_currentSessionPath;
	PowerSpectrumData m_data;
	BaselineInfoPage* m_researcherPage;

};
#endif // BASELINE_INFO_PAGE_CONTROLLER_H
