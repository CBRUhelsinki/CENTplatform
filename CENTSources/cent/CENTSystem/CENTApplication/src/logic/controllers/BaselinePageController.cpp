#include "BaselinePageController.h"

#include "CentActions.h"
#include "Connect.h"
#include "SimplePage.h"
#include "OpenVibeConstants.h"
#include "SetupCapPage.h"
#include "CentConstants.h"

#include <QTimer>

namespace
{
	const QString PLUS_CONTENT = "<html><head></head><body><center><img src=':/images/Global/focusOnPlus.png' width='100' height='100' /></center></body></html>";
	const QString BASELINE_PATIENT_PAGE_NAME = "BaselinePatientPageName";

	const int GAP_BEFORE_RECORD_SEC = 10; // sec
	const int GAP_AFTER_RECORD_SEC  = 10; // sec

	const int GAP_BEFORE_RECORD_MSEC = GAP_BEFORE_RECORD_SEC * 1000; // msec
	const int GAP_AFTER_RECORD_MSEC  = GAP_AFTER_RECORD_SEC * 1000; // msec
}

BaselinePageController::BaselinePageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_researcherPage(0)
	, m_recordTime(1)
	, m_timer(new QTimer(this))
	, m_shootMethod()
{
	m_timer->setSingleShot(true);
}

BaselinePageController::~BaselinePageController()
{
}

bool BaselinePageController::activatePage()
{
	stimulateScenario(CENT::Stimulation::BASELINE_RECORDING, CENT::Stimulation::OFF);

	QVariantList params;
	params.append(OPENVIBE::BaseLine);
	emit invokeAction(CENT::RunOVDesigner, params, this);
	
	timerShoot(GAP_BEFORE_RECORD_MSEC, SLOT(onStartBaseline()));
	m_researcherPage->setState(SetupCapPage::BLPrepare);
	m_researcherPage->startCountdown(GAP_BEFORE_RECORD_SEC);

	return BaseCentPageController::activatePage();
}

bool BaselinePageController::deactivatePage() 
{
	timerStop();
	emit invokeAction(CENT::StopOVDesigner, QVariantList());
	return BaseCentPageController::deactivatePage();
}

void BaselinePageController::result(int err)
{
	if(err != 0)
	{
		QString errText  = QString("Failed to start OpenVibe Designer. Error code: %1").arg(err);
		emit error(errText);
	}
}

void BaselinePageController::onDataReceived(const CentData::AnalogData& data)
{
	m_researcherPage->onDataReceived(data);
}

void BaselinePageController::onDataReceived_5_8(const CentData::AnalogData& data)
{
	m_researcherPage->onDataReceived_5_8(data);
}

void BaselinePageController::onNoiseLevel(const CentData::AnalogData& data)
{
	m_researcherPage->onNoiseLevel(data);
}

void BaselinePageController::onProcessingEnd()
{
	emit goToState(CentBaseLineInfoState);
}

void BaselinePageController::onStartBaseline()
{
	stimulateScenario(CENT::Stimulation::BASELINE_RECORDING, CENT::Stimulation::ON);
	m_researcherPage->setState(SetupCapPage::BLRecording);

	timerShoot(m_recordTime * 60000, SLOT(onBaselineTimeEnd()));
	m_researcherPage->startCountdown(m_recordTime * 60);
}

void BaselinePageController::onBaselineTimeEnd()
{
	timerStop(); // in case it was called by sth else than timeout (ie. "End Button")
	
	//Stimulate scenario to stop bseline recording
	stimulateScenario(CENT::Stimulation::BASELINE_RECORDING, CENT::Stimulation::OFF);
	m_researcherPage->setState(SetupCapPage::BLPostProcessing);

	//TODO: post process baseline results
	emit invokeAction(CENT::StopOVDesigner, QVariantList());
	QVariantList params;
	params.append(OPENVIBE::BaseLinePostProcess);
	emit invokeAction(CENT::RunOVDesigner, params, this);


	//TODO: should be more definitive way of knowing when postprocessing is done, not a timer
	timerShoot(GAP_AFTER_RECORD_MSEC, SLOT(onProcessingEnd()));
	m_researcherPage->startCountdown(GAP_AFTER_RECORD_SEC);
}

void BaselinePageController::onRecordTimeChanged(int time)
{
	if(time > 0)
	{
		m_recordTime = time;
	}
}

ICentPage* BaselinePageController::createResearcherPage()
{
	m_researcherPage = new SetupCapPage(tr("Baseline recording"), SetupCapPage::BaselineResearcher);
	CENT::connect(m_researcherPage, SIGNAL(endClicked()), this, SLOT(onBaselineTimeEnd()));

	return m_researcherPage;
}

ICentPage* BaselinePageController::createPatientPage()
{
	SimplePage* page = new SimplePage("");
	page->setObjectName(BASELINE_PATIENT_PAGE_NAME);
	page->setContent(PLUS_CONTENT, Qt::AlignVCenter);

	return page;
}

void BaselinePageController::stimulateScenario(int channel, int state)
{
	QVariantList list;
	list.append(channel);
	list.append(state);
	emit invokeAction(CENT::SetStimulationButton, list);
}

void BaselinePageController::timerStop()
{
	m_timer->stop();
	if(!m_shootMethod.isEmpty())
	{
		CENT::disconnect(m_timer, SIGNAL(timeout()), this, m_shootMethod.toAscii());
		m_shootMethod.clear();
	}
}

void BaselinePageController::timerShoot(int msec, const char* method)
{
	timerStop();
	CENT::connect(m_timer, SIGNAL(timeout()), this, method);
	m_shootMethod = method;

	m_timer->setInterval(msec);
	m_timer->start();
}
