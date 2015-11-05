#ifndef EVALUATION_PAGE_H
#define EVALUATION_PAGE_H

#include "CentPatientData.h"
#include "ICentPage.h"

class EvaluationQuestionaryUi;

class EvaluationPage: public ICentPage
{
	Q_OBJECT
public:
	explicit EvaluationPage(QWidget* parent = 0);
	virtual ~EvaluationPage();

public:
	virtual void activate();
	virtual void deactivate();

	virtual QString headerText();

signals:
	void nextClicked();
	void questionaryFilled(const CentPatient::EvaluationData& data);
	void wizardPageChanged(int);

private:
	void createLayout();
	void createConnections();

	EvaluationQuestionaryUi* m_questionary;
};


#endif EVALUATION_PAGE_H
