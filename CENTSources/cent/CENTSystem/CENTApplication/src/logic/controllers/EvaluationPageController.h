#ifndef EVALUATION_PAGE_CONTROLLER_H
#define EVALUATION_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "CentPatientData.h"
#include "IResultReceiver.h"

class EvaluationPage;
class DynamicDataModel;

class EvaluationPageController : public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit EvaluationPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~EvaluationPageController();

	virtual void result(int err);

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private slots:
	void onQuestionaryFilled(const CentPatient::EvaluationData& data);

private:
	EvaluationPage* createEvaluationPage();

	DynamicDataModel* m_dataModel;
};
#endif // EVALUATION_PAGE_CONTROLLER_H
