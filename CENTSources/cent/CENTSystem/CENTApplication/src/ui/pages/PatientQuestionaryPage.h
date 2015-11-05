#ifndef CREATE_USER_PAGE_H
#define CREATE_USER_PAGE_H

#include "ICentPage.h"
#include "CentPatientData.h"

class PatientQuestionaryUI;

class PatientQuestionaryPage: public ICentPage
{
	Q_OBJECT
public:
	explicit PatientQuestionaryPage(QWidget* parent = 0);
	virtual ~PatientQuestionaryPage();

signals:
	void questionaryFilled(CentPatient::Data data);
	void goPageBack();
	void userExistsCheckRequest(const QString& userName);

public slots:
	void onNextPage();

public:
	virtual void activate();
	virtual void deactivate();
	void enableContinue(bool enabled);

	virtual QString headerText();

private:
	void createLayout();
	void createConnections();

private:
	Q_DISABLE_COPY(PatientQuestionaryPage)

	PatientQuestionaryUI*    m_patientQuestionaryUI;
};

#endif // CREATE_USER_PAGE_H

