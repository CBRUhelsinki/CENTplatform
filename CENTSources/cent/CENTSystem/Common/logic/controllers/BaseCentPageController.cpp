#include "BaseCentPageController.h"
#include "CentMainWindow.h"
#include "ICentPage.h"

BaseCentPageController::BaseCentPageController(CentMainWindow& researcherWindow,CentMainWindow& patientWindow, QObject* parent)
	: QObject(parent)
	, m_researcherWindow(researcherWindow)
	, m_researcherPage(0)
	, m_patientWindow(patientWindow)
	, m_patientPage(0)
{
}

BaseCentPageController::~BaseCentPageController()
{
}

bool BaseCentPageController::activatePage(CentMainWindow& mainWindow, ICentPage* page)
{
	mainWindow.activatePage(page);
	loadHeaders(mainWindow, page);
	page->activate();
	return true;
}

void BaseCentPageController::loadHeaders(CentMainWindow& mainWindow, ICentPage* page)
{
	QString header = page->headerText();
	mainWindow.setHeaderText(header);
	mainWindow.setHeaderVisible( !header.isEmpty());
}

void BaseCentPageController::loadHeaders()
{
	ICentPage* patientPage = getPatientPage();
	ICentPage* researcherPage = getResearcherPage();

	if (patientPage && researcherPage)
	{
		loadHeaders(m_researcherWindow, researcherPage);
		loadHeaders(m_patientWindow, patientPage);
	}
}

bool BaseCentPageController::activatePage()
{
	ICentPage* patientPage = getPatientPage();
	ICentPage* researcherPage = getResearcherPage();

	if (patientPage && researcherPage)
	{
		activatePage(m_researcherWindow, researcherPage);
		activatePage(m_patientWindow, patientPage);

		return true;
	}

	return false;
}

bool BaseCentPageController::deactivatePage()
{
	ICentPage* patientPage = getPatientPage();
	ICentPage* researcherPage = getResearcherPage();
	if (researcherPage && patientPage)
	{
		patientPage->deactivate();
		researcherPage->deactivate();
		return true;
	}

	return false;
}

bool BaseCentPageController::registerPage()
{
	return m_researcherWindow.addPage(getResearcherPage()) && m_patientWindow.addPage(getPatientPage());
}

ICentPage* BaseCentPageController::getResearcherPage()
{
	if(! m_researcherPage)
	{
		m_researcherPage = createResearcherPage();
	}
	
	return  m_researcherPage;
}

ICentPage* BaseCentPageController::getPatientPage()
{
	if(! m_patientPage)
	{
		m_patientPage = createPatientPage();
	}
	
	return  m_patientPage;
}
