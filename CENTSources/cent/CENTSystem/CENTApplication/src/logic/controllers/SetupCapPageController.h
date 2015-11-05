#ifndef SETUP_CAP_PAGE_CONTROLLER_H
#define SETUP_CAP_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "CentDataTypes.h"
#include "IResultReceiver.h"

class ICentPage;
class SetupCapPage;

class SetupCapPageController : public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit SetupCapPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~SetupCapPageController();

public:
	virtual bool activatePage();
	virtual bool deactivatePage();

	virtual void result(int err);

public slots:
	void onDataReceived(const CentData::AnalogData& data);
	void onDataReceived_5_8(const CentData::AnalogData& data);
	void onNoiseLevel(const CentData::AnalogData& noise);
	void onSkipBaseline();
	void onMeasureBaseline();

signals:
	void dataReceived(const CentData::AnalogData& data);
	void dataReceived_5_8(const CentData::AnalogData& data);
	void noiseLevel(const CentData::AnalogData& noise);
	void baselineMinutesChanged(int);

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private:
	Q_DISABLE_COPY(SetupCapPageController);
};

#endif // SETUP_CAP_PAGE_CONTROLLER_H

