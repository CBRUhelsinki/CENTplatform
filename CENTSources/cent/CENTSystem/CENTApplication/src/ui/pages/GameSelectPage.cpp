#include "GameSelectPage.h"

#include "Connect.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QSignalMapper>

namespace
{
	const int BUTTON_WIDTH = 100;
	const char* const CHOOSE_GAME_TEXT         = "Choose A Game";

	const char* TUTORIAL_PUSH_BUTTON_TEXT      = "See tutorial";
	const char* START_GAME_PUSH_BUTTON_TEXT    = "Start game";
	const char* SETTINGS_GAME_PUSH_BUTTON_TEXT = "Settings";

//	const QString BORDER_STYLE = "#GameElementWidget{border: 3px solid gray;border-radius: 20px;background: white;}";
	const QString GAME_ELEMENT_OBJECT_NAME = "GameElementWidget";
}

GameSelectPage::GameSelectPage(QWidget* parent)
	: ICentPage(parent)
	, m_scrollArea(0)
	, m_gameStartMapper(0)
	, m_gameTutorialMapper(0)
	, m_gameSettingsMapper(0)
{
	createLayout();
}

GameSelectPage::~GameSelectPage()
{
}

QString GameSelectPage::headerText()
{
	return tr(CHOOSE_GAME_TEXT);
}

void GameSelectPage::setAvailableGames(QMap<QString, bool>& games, bool isResearcher)
{
	if(games.isEmpty())
	{
		return;
	}

	if(m_gameStartMapper)
	{
		CENT::disconnect(m_gameStartMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(startGame(QString)));
		delete m_gameStartMapper;
		m_gameStartMapper = 0;
	}

	if(m_gameTutorialMapper)
	{
		CENT::disconnect(m_gameTutorialMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(showGameTutorial(QString)));
		delete m_gameTutorialMapper;
		m_gameTutorialMapper = 0;
	}

	if(m_gameSettingsMapper)
	{
		CENT::disconnect(m_gameSettingsMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(showSettingsGame(QString)));
		delete m_gameSettingsMapper;
		m_gameSettingsMapper = 0;
	}

	// Create signal mappers
	m_gameStartMapper = new QSignalMapper(this);
	m_gameTutorialMapper = new QSignalMapper(this);
	if(isResearcher)
	{
		m_gameSettingsMapper = new QSignalMapper(this);
		CENT::connect(m_gameSettingsMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(showSettingsGame(const QString&)));
	}

	CENT::connect(m_gameStartMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(startGame(const QString&)));
	CENT::connect(m_gameTutorialMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(showGameTutorial(const QString&)));

	// We need to create this widget here because it's needed 
	// to be layed out completely when we call QScrollArea::setWidget();
	QWidget* gameSelectorWidget = new QWidget(this);
	gameSelectorWidget->setObjectName("GameSelectorWidget");
	QVBoxLayout* gameSelectorLayout = new QVBoxLayout();
//	gameSelectorWidget->setStyleSheet(BORDER_STYLE);

	QMapIterator<QString, bool> game(games);
	while (game.hasNext())
	{
		game.next();
		bool isConfigurable = game.value() & isResearcher;
		addGameElement(game.key(), isConfigurable, gameSelectorLayout);
	}

	gameSelectorWidget->setLayout(gameSelectorLayout);

	gameSelectorWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

	// No memory leak, setWidget destroys all child widgets of previous widget
	m_scrollArea->setWidget(gameSelectorWidget);

	// Because scroll area is visible by now we have to call explicitly show.
	// It's recommended by the documentation.
	gameSelectorWidget->show();
}

void GameSelectPage::createLayout()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	// ScrollArea placement
	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidgetResizable(true);
	mainLayout->addWidget(m_scrollArea);

	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
}

void GameSelectPage::addGameElement(const QString& gameName, bool isConfigurable, QVBoxLayout* gameSelectorLayout)
{
	if(! gameSelectorLayout)
	{
		return;
	}

	QHBoxLayout* gameElementLayout = new QHBoxLayout();
	QFrame* gameElement = new QFrame();

	QLabel* gameNameLabel = new QLabel(gameName);
	gameElementLayout->addWidget(gameNameLabel);

	// Create game buttons
	QPushButton* gameStartPushButton = new QPushButton(tr(START_GAME_PUSH_BUTTON_TEXT));
	gameStartPushButton->setFixedWidth(BUTTON_WIDTH);
	QPushButton* seeGameTutorialPushButton = new QPushButton(tr(TUTORIAL_PUSH_BUTTON_TEXT));
	seeGameTutorialPushButton->setFixedWidth(BUTTON_WIDTH);

	// Map button to game name
	m_gameStartMapper->setMapping(gameStartPushButton, gameName);
	m_gameTutorialMapper->setMapping(seeGameTutorialPushButton, gameName);

	CENT::connect(gameStartPushButton, SIGNAL(clicked()), m_gameStartMapper, SLOT(map()));
	CENT::connect(seeGameTutorialPushButton, SIGNAL(clicked()), m_gameTutorialMapper, SLOT(map()));

	// Layout game buttons
	gameElementLayout->addStretch();
	if(isConfigurable)
	{
		QPushButton* settingsGamePushButton = new QPushButton(tr(SETTINGS_GAME_PUSH_BUTTON_TEXT));
		settingsGamePushButton->setFixedWidth(BUTTON_WIDTH);
		CENT::connect(settingsGamePushButton, SIGNAL(clicked()), m_gameSettingsMapper, SLOT(map()));
		m_gameSettingsMapper->setMapping(settingsGamePushButton, gameName);
		gameElementLayout->addWidget(settingsGamePushButton);
	}
	gameElementLayout->addWidget(gameStartPushButton);
	gameElementLayout->addWidget(seeGameTutorialPushButton);
	gameElement->setLayout(gameElementLayout);

	// Set object name for styling
	gameElement->setObjectName(GAME_ELEMENT_OBJECT_NAME);

	gameSelectorLayout->addWidget(gameElement);
}