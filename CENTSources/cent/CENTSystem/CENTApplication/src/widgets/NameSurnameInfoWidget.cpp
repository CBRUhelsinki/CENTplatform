#include "NameSurnameInfoWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QVBoxLayout>

#include "connect.h"
#include "CentPatientData.h"
#include "DataModelCommon.h"

namespace
{
	const char* NAME_FIELD_TEXT("Name:");
	const char* LAST_NAME_FIELS_TEXT("Last Name:");

	const QString USERNAME_SEPARATOR("_");

	const int MAX_NAME_LENGTH = 50;
	const int MAX_LAST_NAME_LENGTH = 50;
}

NameSurnameInfoWidget::NameSurnameInfoWidget(QWidget* parent)
	: QWidget(parent)
	, m_name(0)
	, m_lastName(0)
{
	setObjectName("NameSurnameInfoWidget");
	createLayout();
	createConnections();
	emit isValid(false);
}

NameSurnameInfoWidget::~NameSurnameInfoWidget()
{

}

void NameSurnameInfoWidget::getQuestionaryData(CentPatient::Data& data)
{
	data.m_userName = m_name->text() + USERNAME_SEPARATOR + m_lastName->text();
}

void NameSurnameInfoWidget::reset()
{
	m_name->setText("");
	m_lastName->setText("");
}

void NameSurnameInfoWidget::triggerValidityCheck()
{
	emit isValid(checkValid());
}

void NameSurnameInfoWidget::onChange()
{
	triggerValidityCheck();
}

bool NameSurnameInfoWidget::checkValid()
{
	if(m_name->text().isEmpty() || m_lastName->text().isEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void NameSurnameInfoWidget::createConnections()
{
	CENT::connect(m_name, SIGNAL(textChanged(const QString&)), this, SLOT(onChange()));
	CENT::connect(m_lastName, SIGNAL(textChanged(const QString&)), this, SLOT(onChange()));
}

void NameSurnameInfoWidget::createLayout()
{
	m_name = new QLineEdit;
	m_name->setObjectName(DataModel::ObjectNames::QUESTIONARY_PATIENT_FIRST_NAME);
	
	m_lastName = new QLineEdit;
	m_lastName->setObjectName(DataModel::ObjectNames::QUESTIONARY_PATIENT_LAST_NAME);

	QRegExpValidator* regexp = new QRegExpValidator(QRegExp("^[a-zA-Z0-9\\_]+$"), this);
	m_name->setValidator(regexp);
	m_lastName->setValidator(regexp);

	QVBoxLayout* nameLayout = new QVBoxLayout;
	nameLayout->addStretch();
	nameLayout->addWidget(new QLabel(tr(NAME_FIELD_TEXT)));
	nameLayout->addWidget(m_name);
	nameLayout->addWidget(new QLabel(tr(LAST_NAME_FIELS_TEXT)));
	nameLayout->addWidget(m_lastName);
	nameLayout->addStretch();

	QLabel* imageLabel = new QLabel;
	imageLabel->setObjectName("imageLabel");
	QVBoxLayout* imageLayout = new QVBoxLayout;
	imageLayout->addStretch();
	imageLayout->addWidget(imageLabel);
	imageLayout->addStretch();

	m_name->setMaxLength(MAX_NAME_LENGTH);
	m_lastName->setMaxLength(MAX_LAST_NAME_LENGTH);

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addLayout(imageLayout);
	layout->addStretch();
	layout->addLayout(nameLayout);

	setLayout(layout);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}
