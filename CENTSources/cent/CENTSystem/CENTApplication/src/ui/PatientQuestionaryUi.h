#ifndef PATIENT_UI_H
#define PATIENT_UI_H

#include <QWidget>
#include <QMap>

#include "CentPatientData.h"

class IInfoWidget;
class QPushButton;
class QStackedWidget;
class NameSurnameInfoWidget;

class PatientQuestionaryUI : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int currentWizardPage READ currentPage WRITE setCurrentPage RESET reset NOTIFY currentPageChanged USER true)

public:
	explicit PatientQuestionaryUI(QWidget* parent = 0);
	virtual ~PatientQuestionaryUI();

	void reset();
	void getQuestionaryData(CentPatient::Data& data);
	void enableContinue(bool enable = true);
	int currentPage();

signals:
	void valid(bool isValid);
	void questionaryFilled(CentPatient::Data data);
	void currentPageChanged(int page);
	void goPageBack();
	void userExistsCheckRequest(const QString& userName);

public slots:
	void goToNextPage();

private slots:
	void onContinueButtonClicked();
	void onBackButtonClicked();
	void onQuestionaryUiValidChanged(bool isValid);
	void setCurrentPage(int page);

private:
	void createLayout();
	void createWizardPages(QStackedWidget* wizard);
	void createConnections();

private:
	QList<IInfoWidget*> m_pageWidgets;

	QStackedWidget* m_wizardWidget;
	QPushButton*    m_continueButton;

	CentPatient::Data      m_data;
	NameSurnameInfoWidget* m_nameSurnameInfo;
};

#endif // PATIENT_UI_H
