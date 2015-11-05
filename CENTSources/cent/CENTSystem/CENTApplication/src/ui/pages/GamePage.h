#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <QPointer>

#include "ICentPage.h"
#include "CentDataTypes.h"

class QLabel;

class ICentGamePlugin;
class PowerLevelBar;
class PowerBarHistoryPlot;

class GamePage: public ICentPage
{
	Q_OBJECT
public:
	explicit GamePage(QWidget* parent = 0);
	virtual ~GamePage();

public:
	virtual void activate();
	virtual void deactivate();
	void setGameTimeoutMinutes(int minutes);

public slots:
	void setGameWidget(QWidget* gamePlugin);
	void onUserInput(const CentData::DigitalData& data);
	void onPowerSignal(const CentData::AnalogData& data);
	void onGameEnded();

signals:
	void userInput(const CentData::DigitalData& data);

private:
	void reset();
	void createLayout();
	void setDebugMsg(const QString& text);

private:
	QPointer<QWidget>         m_gameWidget;
	QPointer<QWidget>         m_gamePluginWidget;
	QLabel*                   m_userInput;
	PowerLevelBar*            m_leftPowerBar;
	PowerLevelBar*            m_rightPowerBar;
	PowerBarHistoryPlot*      m_powerBarHistoryPlot;
	bool                      m_gameEnded;
};

#endif // GAME_PAGE_H

