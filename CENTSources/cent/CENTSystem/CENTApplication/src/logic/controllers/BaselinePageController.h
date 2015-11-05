#ifndef BASELINE_PAGE_CONTROLLER_H
#define BASELINE_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "CentDataTypes.h"
#include "IResultReceiver.h"

class ICentPage;
class QTimer;
class SetupCapPage;

class BaselinePageController: public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit BaselinePageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~BaselinePageController();

	virtual bool activatePage();
	virtual bool deactivatePage();

	virtual void result(int err);

signals:
	void baselineRecordingStarted();
	void baselineRecordingStopped();

public slots:
	void onDataReceived(const CentData::AnalogData& data);
	void onDataReceived_5_8(const CentData::AnalogData& data);
	void onNoiseLevel(const CentData::AnalogData& data);
	void onProcessingEnd();
	void onRecordTimeChanged(int time);

	void onStartBaseline();
	void onBaselineTimeEnd();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private:
	void stimulateScenario(int channel, int state);
	void timerStop();
	void timerShoot(int msec, const char* method);

private:
	QString  m_shootMethod;
	int      m_recordTime;
	QTimer*  m_timer;

	SetupCapPage* m_researcherPage;
};

#endif // BASELINE_PAGE_CONTROLLER_H

