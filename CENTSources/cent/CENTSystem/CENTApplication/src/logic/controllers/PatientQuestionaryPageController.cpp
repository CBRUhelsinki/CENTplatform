#include "PatientQuestionaryPageController.h"

#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "CentFileHelpers.h"
#include "CentMainWindow.h"
#include "Connect.h"
#include "DynamicDataModel.h"
#include "PatientDataCenter.h"
#include "PatientQuestionaryPage.h"

PatientQuestionaryPageController::PatientQuestionaryPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow,
																   PatientDataCenter& centPatientDataCenter, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_centPatientDataCenter(centPatientDataCenter)
	, m_researcherPage(0)
	, m_patientPage(0)
	, m_dynamicDataModel(new DynamicDataModel(this))
{
}

PatientQuestionaryPageController::~PatientQuestionaryPageController()
{
}

void PatientQuestionaryPageController::onPatientDataSaveFailed()
{
	m_researcherPage->enableContinue(true);
	m_patientPage->enableContinue(true);

	emit error(tr("Patient data save failed. Please try again"));
}

void PatientQuestionaryPageController::onUserExistsCheckRequest(const QString& userName)
{
	m_researcherPage->enableContinue(false);
	m_patientPage->enableContinue(false);

	QDir dir;
	QString patientDir = m_centPatientDataCenter.patientDirectory(userName);
	if (! dir.exists(patientDir))
	{
		if (! dir.mkdir(patientDir))
		{
			emit error(tr("Cannot create directory for current patient."));
			return;
		}
	}

	bool needToCopyIEPFiles = false;
	QString iepDir = m_centPatientDataCenter.patientIEPDirectory(userName);
	if (! dir.exists(iepDir))
	{
		needToCopyIEPFiles = true;

		if (! dir.mkdir(iepDir))
		{
			emit error(tr("Cannot create IEP directory for current patient."));
			return;
		}
	}

	if (! needToCopyIEPFiles)
	{
		if (m_centPatientDataCenter.isValidIEPDirectory(iepDir))
		{
			qDebug() << "Nothing to do, IEP folder exists, we assume that this is enough for now.";
			emit goToNextPage();
			return;
		}
	}

	QMessageBox msgBox(QMessageBox::Information,
		tr("IEP directory"),
		tr("Choose IEP directory for current patient"),
		QMessageBox::Ok | QMessageBox::Cancel,
		m_researcherPage);

	if (QMessageBox::Ok == msgBox.exec())
	{
		QString sourceDirectory = QFileDialog::getExistingDirectory(m_researcherPage,
			tr("Choose IEP directory for current patient"),
			"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		//TODO: what is below should be encapsulated in CentPatientDataCenter, called through signal/slot mechanism
		if (! sourceDirectory.isEmpty())
		{
			if (m_centPatientDataCenter.isValidIEPDirectory(sourceDirectory))
			{
				if (dir.exists(sourceDirectory))
				{
					if (CentFileHelpers::copyFolder(sourceDirectory, iepDir))
					{
						emit goToNextPage();
						return;
					}
				}
				emit error(tr("Cannot copy IEP files. Please try again."));
			}
			else
			{
				emit error(tr("Specified directory does not contain IEP files."));
			}
		}
	}

	// Remove created directories if empty
	dir.rmdir(iepDir);
	dir.rmdir(patientDir);

	m_researcherPage->enableContinue(true);
	m_patientPage->enableContinue(true);
}

ICentPage* PatientQuestionaryPageController::createResearcherPage()
{
	m_researcherPage = createQuestionaryPage();
	CENT::connect(this, SIGNAL(goToNextPage()), m_researcherPage, SLOT(onNextPage()));
	return static_cast<ICentPage*>(m_researcherPage);
}

ICentPage* PatientQuestionaryPageController::createPatientPage()
{
	m_patientPage = createQuestionaryPage();
	return static_cast<ICentPage*>(m_patientPage);
}

PatientQuestionaryPage* PatientQuestionaryPageController::createQuestionaryPage()
{
	PatientQuestionaryPage* patientPage = new PatientQuestionaryPage();
	CENT::connect(patientPage, SIGNAL(questionaryFilled(CentPatient::Data)), this, SIGNAL(questionaryFilled(CentPatient::Data)));
	CENT::connect(patientPage, SIGNAL(goPageBack()), this, SIGNAL(goPageBack()));
	CENT::connect(patientPage, SIGNAL(userExistsCheckRequest(const QString&)), this, SLOT(onUserExistsCheckRequest(const QString&)));

	m_dynamicDataModel->addToModel(patientPage);
	return patientPage;
}
