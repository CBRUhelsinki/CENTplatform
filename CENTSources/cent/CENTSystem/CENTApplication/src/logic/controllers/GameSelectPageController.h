#ifndef GAME_SELECT_PAGE_CONTROLLER_H
#define GAME_SELECT_PAGE_CONTROLLER_H

#include "BaseCentPageController.h"
#include "IResultReceiver.h"
#include "StateEnums.h"

class ICentPage;
class GameSelectPage;

class GameSelectPageController: public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit GameSelectPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent = 0);
	virtual ~GameSelectPageController();

public:
	void setAvailableGames(QMap<QString, bool>& games);
	virtual void result(int error_code);

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

	virtual GameSelectPage* createGameSelectPage();

private slots:
	void onStartGame(QString gameName);
	void onShowGameTutorial(QString gameName);
	void onShowSettingsGame(QString gameName);

private:
	Q_DISABLE_COPY(GameSelectPageController);

	GameSelectPage* m_researcherGameSelectPage;
	GameSelectPage* m_patientGameSelectPage;
};
#endif // GAME_SELECT_PAGE_CONTROLLER_H

