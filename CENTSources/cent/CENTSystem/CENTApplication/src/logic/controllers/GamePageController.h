#ifndef GAME_PAGE_CONTROLLER_H
#define GAME_PAGE_CONTROLLER_H

#include <QPointer>

#include "BaseCentPageController.h"
#include "CentDataTypes.h"
#include "CentPatientData.h"
#include "IResultReceiver.h"

class CentGameManager;
class GamePage;
class SetupCapPage;
class ICentGamePlugin;
class ICentPage;
class SummaryController;
class QTimer;

class GamePageController: public BaseCentPageController, public IResultReceiver
{
	Q_OBJECT
public:
	explicit GamePageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, SummaryController& summaryController, QObject* parent = 0);
	virtual ~GamePageController();

public:
	virtual bool activatePage();
	virtual bool deactivatePage();

	virtual void result(int err);

public slots:
	void onUserInput(const CentData::DigitalData& data);
	void onPowerSignal(const CentData::AnalogData& data);
	void onDataReceived(const CentData::AnalogData& data);
	void onDataReceived_5_8(const CentData::AnalogData& data);
	void onNoiseLevel(const CentData::AnalogData& data);

	void onGameStarted();
	void onGameEnded();

	void onReleaseGamePluginWidget();
	void onGamePluginChanged(ICentGamePlugin* gamePlugin, QString fileName);
	void onGameTimeoutSet(int minutes);

signals:
	void stopGame();
	void startGame();

	void powerSignal(CentData::AnalogData data);
	void userInput(CentData::DigitalData data);
	void expectedInput(const CentData::DigitalData& data);

	void gameStarted(CentPatient::GameData gameData);
	void gameEnded(CentPatient::SummaryData gameData);

private slots:
	void onEndGame();

protected:
	virtual ICentPage* createResearcherPage();
	virtual ICentPage* createPatientPage();

private:
	Q_DISABLE_COPY(GamePageController)
	
	QPointer<ICentGamePlugin>       m_currentGame;
	QPointer<QWidget>               m_currentGameWidget;
	GamePage*                       m_patientGamePage;
	SetupCapPage*                   m_researcherPage;
	SummaryController&              m_summaryController;
	CentPatient::GameData           m_currentGameData;
	QTimer*                         m_gameTimeoutTimer;

	bool                            m_gameStarted;
	int                             m_gameTimeoutMinutes;
	bool                            m_manualStop; // game being stopped from cent not by it's internal controls
};
#endif // GAME_PAGE_CONTROLLER_H
