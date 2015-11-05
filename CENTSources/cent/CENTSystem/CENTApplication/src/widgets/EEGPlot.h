#ifndef EEGPLOT_H
#define EEGPLOT_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include "qwt_plot.h" 
#include "CentDataTypes.h"


class QwtPlotCurve;
class QwtPlotDirectPainter;
class ArraySeriesData;

class EEGPlot: public QwtPlot
{
	Q_OBJECT
public:
	explicit EEGPlot(int channelId, QWidget *parent, const QObject* sender,
	                 const char* dataSignal, unsigned int numChannels);

	virtual ~EEGPlot();
	
public slots:
	void dataReceived(const CentData::AnalogData& data);
	void replotManual();
	void reset();
	void refreshScale();

private:
	void drawSample(const CentData::AnalogData& data);

private:
	//<todo> doxygen
	int          m_channelId;// current channel id

	bool m_firstPacket; // is this the first packet

	ArraySeriesData* m_arraySeriesData; // data holder for the EEG curve
	QwtPlotCurve* m_curve; // EEG curve drawer
	QwtPlotDirectPainter* m_directPainter; // for triggering the repaint
	QTimer m_redrawTimer; // we want to redraw in a fps manner
	unsigned int m_numChannels;
};

#endif // EEGPLOT_H

