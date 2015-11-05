#ifndef CREATE_USER_PAGE_CONTROLLER_H
#define CREATE_USER_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "CentPatientData.h"

class DynamicDataModel;
class ICentPage;
class PatientDataCenter;
class PatientQuestionaryPage;

class PatientQuestionaryPageController: public BaseCentPageController
{
	Q_OBJECT
public:
	PatientQuestionaryPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow,
		PatientDataCenter& centPatientDataCenter, QObject* parent = 0);
	virtual ~PatientQuestionaryPageController();

public slots:
	void onPatientDataSaveFailed();
	void onUserExistsCheckRequest(const QString& userName);

signals:
	void questionaryFilled(CentPatient::Data);
	void goPageBack();
	void goToNextPage();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

	virtual PatientQuestionaryPage* createQuestionaryPage();

private:
	Q_DISABLE_COPY(PatientQuestionaryPageController);

	PatientDataCenter& m_centPatientDataCenter;
	DynamicDataModel* m_dynamicDataModel;

	PatientQuestionaryPage* m_researcherPage;
	PatientQuestionaryPage* m_patientPage;
};

#endif // CREATE_USER_PAGE_CONTROLLER_H

