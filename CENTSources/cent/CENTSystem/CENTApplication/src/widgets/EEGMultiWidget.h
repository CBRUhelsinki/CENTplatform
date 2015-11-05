#ifndef EEGMULTIWIDGET_H
#define EEGMULTIWIDGET_H

#include <QWidget>
#include "CentDataTypes.h"

class EEGPlot;
class QVBoxLayout;

class EEGMultiWidget: public QWidget
{
	Q_OBJECT
public:
	explicit EEGMultiWidget(QWidget *parent=0);
	virtual ~EEGMultiWidget();

	void configureLayout();

signals:
	void dataReceived(const CentData::AnalogData& data);
	void resetPlots();
	void refreshScales();

public slots:
	void onReset();
	void onRefreshScales();
	void onDataReceived(const CentData::AnalogData& data);

public:
	bool addPlot(unsigned int chNum, unsigned int numChannels, bool showXScale);

private:
	QVBoxLayout* m_layout;
};

#endif // EEGMULTIWIDGET_H

