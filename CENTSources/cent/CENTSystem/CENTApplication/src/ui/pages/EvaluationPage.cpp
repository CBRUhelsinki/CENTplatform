#include "EvaluationPage.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "Connect.h"
#include "EvaluationQuestionaryUi.h"

namespace
{
	const int BUTTON_MAX_WIDTH = 100;
}

EvaluationPage::EvaluationPage(QWidget* parent)
	: ICentPage(parent)
	, m_questionary(0)
{
	createLayout();
	createConnections();
}

EvaluationPage::~EvaluationPage()
{
}

void EvaluationPage::activate()
{
}

void EvaluationPage::deactivate()
{
	m_questionary->reset();
}

QString EvaluationPage::headerText()
{
	return m_questionary->headerText();
}

void EvaluationPage::createLayout()
{
	m_questionary = new EvaluationQuestionaryUi();
	m_questionary->setObjectName("EvaluationQuestionaryWidget");

	//main layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(m_questionary);

	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

void EvaluationPage::createConnections()
{
	CENT::connect(m_questionary, SIGNAL(questionaryFilled(const CentPatient::EvaluationData&)), this, SIGNAL(questionaryFilled(const CentPatient::EvaluationData&)));
	CENT::connect(m_questionary, SIGNAL(currentPageChanged(int)), this, SIGNAL(wizardPageChanged(int)));
}
