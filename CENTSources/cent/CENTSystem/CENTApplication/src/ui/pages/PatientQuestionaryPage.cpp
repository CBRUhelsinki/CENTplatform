#include "PatientQuestionaryPage.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>

#include "Connect.h"
#include "DataModelCommon.h"
#include "PatientQuestionaryUi.h"
#include "CentPatientData.h"

namespace
{
	const char* const HEADER_TEXT = "Information For Today's Session";
}

PatientQuestionaryPage::PatientQuestionaryPage(QWidget* parent)
	: ICentPage(parent)
	, m_patientQuestionaryUI(0)
{
	createLayout();
	createConnections();
}

PatientQuestionaryPage::~PatientQuestionaryPage()
{
}

void PatientQuestionaryPage::activate()
{
	m_patientQuestionaryUI->enableContinue(false);
}

void PatientQuestionaryPage::deactivate()
{
	m_patientQuestionaryUI->reset();
}

void PatientQuestionaryPage::enableContinue(bool enabled)
{
	m_patientQuestionaryUI->enableContinue(enabled);
}

QString PatientQuestionaryPage::headerText()
{
	return tr(HEADER_TEXT);
}

void PatientQuestionaryPage::onNextPage()
{
	m_patientQuestionaryUI->goToNextPage();
}

void PatientQuestionaryPage::createLayout()
{
	m_patientQuestionaryUI = new PatientQuestionaryUI(this);
	m_patientQuestionaryUI->setObjectName(DataModel::ObjectNames::QUESTIONARY_WIZARD_NAME);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	m_patientQuestionaryUI->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	mainLayout->addWidget(m_patientQuestionaryUI);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setLayout(mainLayout);
}

void PatientQuestionaryPage::createConnections()
{
	CENT::connect(m_patientQuestionaryUI, SIGNAL(questionaryFilled(CentPatient::Data)), this, SIGNAL(questionaryFilled(CentPatient::Data)));
	CENT::connect(m_patientQuestionaryUI, SIGNAL(goPageBack()), this, SIGNAL(goPageBack()));

	CENT::connect(m_patientQuestionaryUI, SIGNAL(userExistsCheckRequest(const QString&)),
		this, SIGNAL(userExistsCheckRequest(const QString&)));
}
