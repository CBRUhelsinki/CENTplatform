#include "SetupCapPageController.h"

#include <QDebug>

#include "CentActions.h"
#include "CentErrors.h"
#include "Connect.h"
#include "OpenVibeConstants.h"
#include "SetupCapPage.h"
#include "StateEnums.h"


SetupCapPageController::SetupCapPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
{
}

SetupCapPageController::~SetupCapPageController()
{
}

bool SetupCapPageController::activatePage()
{
	QVariantList params;
	params.append(OPENVIBE::EEGWithNoise);
	emit invokeAction(CENT::RunOVDesigner, params, this);
	return BaseCentPageController::activatePage();
}

bool SetupCapPageController::deactivatePage() 
{
	emit invokeAction(CENT::StopOVDesigner, QVariantList(), NULL, ControlStateUndefined);
	return BaseCentPageController::deactivatePage();
}

void SetupCapPageController::result(int err)
{
	if(err == BaselineCopyError)
	{
		emit(error(tr("Failed to copy default baseline folder, verify that you provided a full and valid IEP for current patient")));
	}
	else if(err != 0)
	{
		QString errText  = QString("Failed to start OpenVibe Designer. Error code: %1").arg(err);
		emit error(errText);
	}
}

void SetupCapPageController::onDataReceived(const CentData::AnalogData& data)
{
	emit dataReceived(data);
}

void SetupCapPageController::onDataReceived_5_8(const CentData::AnalogData& data)
{
	emit dataReceived_5_8(data);
}

void SetupCapPageController::onNoiseLevel(const CentData::AnalogData& noise)
{
//	qDebug() << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz") << __FUNCTION__;
	emit noiseLevel(noise);
}

void SetupCapPageController::onSkipBaseline()
{
	emit invokeAction(CENT::UseDefaultBaseline, QVariantList(), this, CentBaseLineInfoState);
}

void SetupCapPageController::onMeasureBaseline()
{
	emit goToState(CentBaseLineState);
}

ICentPage* SetupCapPageController::createResearcherPage()
{
	SetupCapPage* page = new SetupCapPage(tr("Setup Cap"), SetupCapPage::SetupCapResearcher);
	CENT::connect(page, SIGNAL(skipBaselineClicked()), this, SLOT(onSkipBaseline()));
	CENT::connect(page, SIGNAL(measureBaselineClicked()), this, SLOT(onMeasureBaseline()));
	CENT::connect(page, SIGNAL(baselineMinutesChanged(int)), this, SIGNAL(baselineMinutesChanged(int)));

	CENT::connect(this, SIGNAL(dataReceived(const CentData::AnalogData&)), page, SLOT(onDataReceived(const CentData::AnalogData&)));
	CENT::connect(this, SIGNAL(dataReceived_5_8(const CentData::AnalogData&)), page, SLOT(onDataReceived_5_8(const CentData::AnalogData&)));
	CENT::connect(this, SIGNAL(noiseLevel(const CentData::AnalogData&)), page, SLOT(onNoiseLevel(const CentData::AnalogData&)));

	return page;
}

ICentPage* SetupCapPageController::createPatientPage()
{
	SetupCapPage* page = new SetupCapPage(tr("Setup Cap"), SetupCapPage::SetupCapPatient);

	CENT::connect(this, SIGNAL(dataReceived(CentData::AnalogData)), page, SLOT(onDataReceived(CentData::AnalogData)));
	CENT::connect(this, SIGNAL(dataReceived_5_8(CentData::AnalogData)), page, SLOT(onDataReceived_5_8(CentData::AnalogData)));
	CENT::connect(this, SIGNAL(noiseLevel(const CentData::AnalogData&)), page, SLOT(onNoiseLevel(const CentData::AnalogData&)));

	return page;
}

