#ifndef Simple_BALL_GAME_PLUGIN_H 
#define Simple_BALL_GAME_PLUGIN_H

#include "ICentGamePlugin.h"
#include "SimpleBallGameLogic.h"

#include <QPointer>

class SimpleBallGameSummary;
class SimpleBallGameWidget;
class GLWidget;
struct GLColor;

class SimpleBallGamePlugin: public ICentGamePlugin
{
	Q_OBJECT
	Q_INTERFACES(ICentGamePlugin)
public:
	SimpleBallGamePlugin();
	virtual ~SimpleBallGamePlugin();

public:
	//from ICentGamePlugin
	virtual QString gameName();
	virtual QWidget* gameWidget();
	virtual void releaseWidget();
	virtual bool isConfigurable() const { return false; }

public slots:
	//from ICentGamePlugin
	virtual void onUserInput(CentData::DigitalData data);
	virtual void onExpectedInput(const CentData::DigitalData& data);
	virtual void onEEG(CentData::AnalogData data);
	virtual void onStartGame();
	virtual void onEndGame();
	virtual void onPowerSignal(CentData::AnalogData data);
	virtual void onShowSettings() {};

private:
	bool m_expectedHigh;
	bool m_highState;

	long m_expectedHighTimestampS;
	long m_expectedHighTimestampUs;
	long m_userInputHighTimestampS;
	long m_userInputHighTimestampUs;

	SimpleBallGameLogic*   m_logic;
	SimpleBallGameSummary* m_summary;

	QPointer<SimpleBallGameWidget> m_glWidget;

private:
	void createConnections();
	void disconnectConnections();

};
#endif //Simple_BALL_GAME_PLUGIN_H
