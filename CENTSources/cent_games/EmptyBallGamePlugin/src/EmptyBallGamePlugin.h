#ifndef Empty_BALL_GAME_PLUGIN_H 
#define Empty_BALL_GAME_PLUGIN_H

#include <QPointer>

#include "ICentGamePlugin.h"
class EmptyBallGameWidget;

class EmptyBallGamePlugin: public ICentGamePlugin
{
	Q_OBJECT
	Q_INTERFACES(ICentGamePlugin)
public:
	EmptyBallGamePlugin();
	virtual ~EmptyBallGamePlugin();

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
	virtual void onPowerSignal(CentData::AnalogData data){Q_UNUSED(data)}
	virtual void onShowSettings() {};

private: 
	bool m_expectedHigh;
	bool m_highState;
	long m_expectedHighTimestampS;
	long m_expectedHighTimestampUs;
	long m_userInputHighTimestampS;
	long m_userInputHighTimestampUs;

	int m_expectedInput;
	int m_userInput;
	QPointer<EmptyBallGameWidget> m_glWidget;

private:
	void createConnections();
	void setBallLight();
};

#endif //Empty_BALL_GAME_PLUGIN_H
