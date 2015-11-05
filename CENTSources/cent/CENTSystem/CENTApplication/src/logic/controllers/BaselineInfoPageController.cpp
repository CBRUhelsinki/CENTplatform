#include "BaselineInfoPageController.h"

#include "BaselineInfoPage.h"
#include "Connect.h"
#include "PowerSpectrumFileReader.h"
#include "SimplePage.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>

namespace
{
	const QString PLEASE_WAIT_HTML_FORMATTER = "<html><head></head><body><center><h1>%1</h1></center></body></html>";
	const QString FILE_NAME = "baseline_spectrum.csv";
}

BaselineInfoPageController::BaselineInfoPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
		: BaseCentPageController(researcherWindow, patientWindow, parent)
		, m_researcherPage(0)
{
}

BaselineInfoPageController::~BaselineInfoPageController()
{
}

void BaselineInfoPageController::setCurrentSessionPath(const QString& path)
{
	m_currentSessionPath = path;
}

void BaselineInfoPageController::setBetaThetaLevels(double beta, double theta)
{
	m_researcherPage->setBetaThetaLevels(beta, theta);
}

void BaselineInfoPageController::resetSession()
{
	m_researcherPage->resetSession();
}

void BaselineInfoPageController::result(int err)
{
	Q_UNUSED(err);
}

bool BaselineInfoPageController::activatePage()
{
	emit invokeAction(CENT::ParseBetaThetaLevelValues, QVariantList());
	PowerSpectrumFileReader fileReader(m_data);

	const QString filePath = m_currentSessionPath + QDir::separator() + FILE_NAME; 
	if (fileReader.readFile(filePath))
	{
		if (m_data.checkDataIntegrity())
		{
			m_researcherPage->setDisplayData(m_data);
		}
		else
		{
		}
	}
	else
	{
		qDebug() << "Failed to read/process specified file: " << filePath;
	}

	return BaseCentPageController::activatePage();
}

ICentPage* BaselineInfoPageController::createResearcherPage()
{
	m_researcherPage = new BaselineInfoPage();

	CENT::connect(m_researcherPage, SIGNAL(reRecord()), this, SLOT(onReRecord()));
	CENT::connect(m_researcherPage, SIGNAL(next()), this, SLOT(onNext()));
	CENT::connect(m_researcherPage, SIGNAL(gameTimeoutChanged(int)), this, SIGNAL(gameTimeoutChanged(int)));
	return m_researcherPage;
}

ICentPage* BaselineInfoPageController::createPatientPage()
{
	SimplePage* page = new SimplePage("");
	page->setContent(PLEASE_WAIT_HTML_FORMATTER.arg(tr("Please Wait...")), Qt::AlignVCenter);

	return page;
}

void BaselineInfoPageController::onReRecord()
{
	emit goToState(CentCapSetupState);
}

void BaselineInfoPageController::onNext()
{
	QVariantList list;
	list.append(m_researcherPage->betaCoeffValue());
	list.append(m_researcherPage->thetaCoeffValue());
	emit invokeAction(CENT::SaveCoeffValues, list, this, CentGameSelectState);
}
