#ifndef SETUP_CAP_PAGE_H
#define SETUP_CAP_PAGE_H

#include "ICentPage.h"
#include "CentDataTypes.h"

#include <QTime>

class QHBoxLayout;
class QLabel;
class QPushButton;

class SetupCapPage: public ICentPage
{
	Q_OBJECT
public:
	enum PageMode
	{
		SetupCapPatient = 0,
		SetupCapResearcher,
		BaselineResearcher,
		GameResearcher
	};

	enum BaselineState
	{
		BLUndefined = 0,
		BLPrepare,
		BLRecording,
		BLPostProcessing
	};

	explicit SetupCapPage(const QString& title, PageMode mode, QWidget* parent = 0);
	virtual ~SetupCapPage();

signals:
	void dataReceived(const CentData::AnalogData& data);
	void dataReceived_5_8(const CentData::AnalogData& data);
	void noiseLevel(const CentData::AnalogData& noise);
	void resetPlots();
	void endClicked();
	void skipBaselineClicked();
	void measureBaselineClicked();
	void baselineMinutesChanged(int);


public slots:
	void onDataReceived(const CentData::AnalogData& data);
	void onDataReceived_5_8(const CentData::AnalogData& data);
	void onNoiseLevel(const CentData::AnalogData& noise);
	void startCountdown(int seconds);

public:
	virtual void activate();
	virtual void deactivate();

	QString headerText();
	void setState(BaselineState state);

private slots:
	void onTick();

private:
	void setupButtons(PageMode mode, QList<QWidget*>& widgets);

private:
	QString              m_title;
	QLabel*              m_stateLabel;
	QLabel*              m_timeLabel;
	QTimer*              m_timer;
	QTime                m_timeLeft;
	QPushButton*         m_endButton;
	CentData::CapDrivers m_capDriverType;
};
#endif // SETUP_CAP_PAGE_H

