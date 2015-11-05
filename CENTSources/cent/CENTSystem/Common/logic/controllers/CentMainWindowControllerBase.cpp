#include "CentMainWindowControllerBase.h"
#include "CentMainWindow.h"
#include "Connect.h"

#include <QPushButton>

CentMainWindowControllerBase::CentMainWindowControllerBase(CentMainWindow& researcherMainWindow, CentMainWindow& patientMainWindow, QObject* parent)
	: QObject(parent)
	, m_researcherMainWindow(researcherMainWindow)
	, m_patientMainWindow(patientMainWindow)
{

}

CentMainWindowControllerBase::~CentMainWindowControllerBase()
{
}

void CentMainWindowControllerBase::closeMainWindow()
{
	m_researcherMainWindow.close();
	m_patientMainWindow.close();
}

void CentMainWindowControllerBase::createConnections()
{
	createConnections(m_researcherMainWindow);
	createConnections(m_patientMainWindow);
}

void CentMainWindowControllerBase::createConnections(CentMainWindow& window)
{
	CENT::connect(&window, SIGNAL(closeClicked()), this, SIGNAL(close()));
}

