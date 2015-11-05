#include "EvaluationPageController.h"

#include "Connect.h"
#include "DynamicDataModel.h"
#include "EvaluationPage.h"
#include "StateEnums.h"

EvaluationPageController::EvaluationPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
{
	m_dataModel = new DynamicDataModel(this);
}

EvaluationPageController::~EvaluationPageController()
{
}

void EvaluationPageController::result(int err)
{
	Q_UNUSED(err);
}

void EvaluationPageController::onQuestionaryFilled(const CentPatient::EvaluationData& data)
{
	QVariantList params;
	QVariant var;
	var.setValue(data);
	params << var;
	emit invokeAction(CENT::SaveEvaluation, params, this, CentFinishState);
}

ICentPage* EvaluationPageController::createResearcherPage()
{
	return createEvaluationPage();
}

ICentPage* EvaluationPageController::createPatientPage()
{
	return createEvaluationPage();
}

EvaluationPage* EvaluationPageController::createEvaluationPage()
{
	EvaluationPage* page = new EvaluationPage();
	m_dataModel->addToModel(page);
	CENT::connect(page, SIGNAL(questionaryFilled(const CentPatient::EvaluationData&)), this, SLOT(onQuestionaryFilled(const CentPatient::EvaluationData&)));
	CENT::connect(page, SIGNAL(wizardPageChanged(int)), this, SLOT(loadHeaders()));
	return page;
}
