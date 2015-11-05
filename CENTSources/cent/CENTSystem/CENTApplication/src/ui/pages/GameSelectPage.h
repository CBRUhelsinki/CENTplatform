#ifndef TGAME_SELECT_PAGE_H
#define GAME_SELECT_PAGE_H

#include "ICentPage.h"

class QScrollArea;
class QWidget;
class QVBoxLayout;
class QSignalMapper;

class GameSelectPage: public ICentPage
{
	Q_OBJECT
public:
	explicit GameSelectPage(QWidget* parent = 0);
	virtual ~GameSelectPage();

signals:
	void startGame(const QString& gameName);
	void showGameTutorial(const QString& gameName);
	void showSettingsGame(const QString& gameName);

public:
	virtual void activate() {};
	virtual void deactivate() {};
	QString headerText();

	void setAvailableGames(QMap<QString, bool>& games, bool isResearcher = false);

private:
	void createLayout();
	void addGameElement(const QString& gameName, bool isConfigurable, QVBoxLayout* gameSelectorLayout);

private:
	QScrollArea*   m_scrollArea;
	QSignalMapper* m_gameStartMapper;
	QSignalMapper* m_gameTutorialMapper;
	QSignalMapper* m_gameSettingsMapper;

private:
	Q_DISABLE_COPY(GameSelectPage)
};
#endif // GAME_SELECT_PAGE_H

