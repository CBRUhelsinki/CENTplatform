#ifndef PATIENT_UI_H
#define PATIENT_UI_H

#include <QWidget>
#include <QMap>

#include "CentPatientData.h"

class QPushButton;
class QStackedWidget;
class MultiButtonWidget;

class EvaluationQuestionaryUi : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int currentWizardPage READ currentPage WRITE setCurrentPage RESET reset NOTIFY currentPageChanged USER true)
public:
	explicit EvaluationQuestionaryUi(QWidget* parent = 0);
	virtual ~EvaluationQuestionaryUi();

	void reset();
	void getQuestionaryData(CentPatient::EvaluationData& data);
	int currentPage();

	QString headerText();

signals:
	void valid(bool isValid);
	void questionaryFilled(const CentPatient::EvaluationData& data);
	void currentPageChanged(int page);
	void goPageBack();

private slots:
	void onContinueButtonClicked();
	void onBackButtonClicked();
	void onQuestionaryUiValidChanged(bool isValid);
	void setCurrentPage(int page);
	void goToNextPage();

private:
	void createLayout();
	void createWizardPages(QStackedWidget* wizard);
	void createConnections();

private:
	QStackedWidget* m_wizardWidget;

	CentPatient::EvaluationData m_data;

	MultiButtonWidget* m_performanceWidget;
	MultiButtonWidget* m_stressWidget;

	QPushButton*       m_continueButton;
	QPushButton*       m_backButton;
};

#endif // PATIENT_UI_H
