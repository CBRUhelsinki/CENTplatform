#include "GameSelectPageController.h"
#include "GameSelectPage.h"
#include "Connect.h"
#include "ICentGamePlugin.h"
#include "CentErrors.h"

#include <QVariantList>
#include <QDebug>

GameSelectPageController::GameSelectPageController(CentMainWindow& researcherWindow, CentMainWindow& patientWindow, QObject* parent)
	: BaseCentPageController(researcherWindow, patientWindow, parent)
	, m_researcherGameSelectPage(0)
	, m_patientGameSelectPage(0)
{

}

GameSelectPageController::~GameSelectPageController()
{
}

void GameSelectPageController::setAvailableGames(QMap<QString, bool>& games)
{
	if(!m_researcherGameSelectPage || ! m_patientGameSelectPage)
	{
		return;
	}
	m_researcherGameSelectPage->setAvailableGames(games, true);
	m_patientGameSelectPage->setAvailableGames(games);
}

void GameSelectPageController::result(int errorCode)
{
	
	if(errorCode == GameManagerUnavailable)
	{
		QString err;
		err = "Game manager is unavailable, cannot select game.";
		qCritical()<< __FUNCTION__ << err;
		emit error(err);
	}
	else if(errorCode == GameUnavailable)
	{
		QString err;
		err = "Selected game is unavailable, cannot select game.";
		qCritical() << __FUNCTION__ << err;
		emit error(err);
	}
	else if(errorCode != Success)
	{
		QString err("Unknown error, cannot select game.");
		qCritical() << __FUNCTION__ << err;
		emit error(err);
	}
}

ICentPage* GameSelectPageController::createResearcherPage()
{
	m_researcherGameSelectPage = createGameSelectPage();
	return m_researcherGameSelectPage;
}

ICentPage* GameSelectPageController::createPatientPage()
{
	m_patientGameSelectPage = createGameSelectPage();
	return m_patientGameSelectPage;
}

GameSelectPage* GameSelectPageController::createGameSelectPage()
{
	GameSelectPage* gameSelectPage = new GameSelectPage();

	CENT::connect(gameSelectPage, SIGNAL(startGame(QString)), this, SLOT(onStartGame(QString)));
	CENT::connect(gameSelectPage, SIGNAL(showGameTutorial(QString)), this, SLOT(onShowGameTutorial(QString)));
	CENT::connect(gameSelectPage, SIGNAL(showSettingsGame(QString)), this, SLOT(onShowSettingsGame(QString)));
	return gameSelectPage;
}

void GameSelectPageController::onStartGame(QString gameName)
{
	QVariantList params;
	params.append(QVariant(gameName));
	invokeAction(CENT::SetCurrentGame,  params, this, CentGameState);

}

void GameSelectPageController::onShowGameTutorial(QString gameName)
{
	QVariantList params;
	params.append(QVariant(gameName));
	invokeAction(CENT::SetCurrentGame, params, this, CentTutorialState);
}

void GameSelectPageController::onShowSettingsGame(QString gameName)
{
	QVariantList params;
	params.append(QVariant(gameName));
	invokeAction(CENT::ShowSettings, params, this);
}
