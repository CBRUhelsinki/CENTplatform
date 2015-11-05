#include "EEGPlot.h"

#include <QTimer>

#include "Connect.h"
#include "ArraySeriesData.h"

#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_scale_draw.h>

namespace
{
	const long SAMPLE_DELTA = 4000;
	const qreal SAMPLE_DELTA_REAL = 0.004; // 4 ms, which comes from 250Hz

	const QwtText Y_AXIS_TEXT = "µV";
	const char* X_AXIS_TEXT_LOC = "Time sec";

	class IntegerScaleDraw : public QwtScaleDraw
	{
	public:
		QwtText label( double value) const
		{
			int val = value;
			return QwtText(QString::number(val));
		}

		virtual double extent( const QFont & ) const
		{
			return 50;
		}
	};
}

EEGPlot::EEGPlot(int channelId, QWidget *parent, const QObject* sender,
				 const char* dataSignal, unsigned int numChannels)
	: QwtPlot(parent)
	, m_channelId(channelId)
	, m_firstPacket(true)
	, m_numChannels(numChannels)
{
	setAutoReplot(false);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	CENT::connect(sender, dataSignal, this, SLOT(dataReceived(CentData::AnalogData)));

	m_curve = new QwtPlotCurve("signal channel");
	m_curve->setStyle(QwtPlotCurve::Lines);
	m_curve->setPen(QPen(Qt::darkBlue));

	m_curve->setRenderHint(QwtPlotItem::RenderAntialiased, false);
	m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, true);
	
	m_arraySeriesData = new ArraySeriesData();
	m_curve->setData(m_arraySeriesData);
	m_curve->attach(this);

	// Canvas
	setCanvasBackground(QBrush(Qt::white));

	// Axes
	enableAxis(QwtPlot::yLeft, true);
	setAxisTitle(QwtPlot::yLeft, Y_AXIS_TEXT);
	setAxisAutoScale(QwtPlot::yLeft, true);
	setAxisScaleDraw(QwtPlot::yLeft, new IntegerScaleDraw);


	setAxisAutoScale(QwtPlot::xBottom, false);
	QwtText xText(tr(X_AXIS_TEXT_LOC));
	setAxisTitle(QwtPlot::xBottom, xText);

	CENT::connect(&m_redrawTimer, SIGNAL(timeout()), this, SLOT(replotManual()));
	m_redrawTimer.setInterval(200);
	m_redrawTimer.start();
}

EEGPlot::~EEGPlot()
{
	m_redrawTimer.stop();
}

void EEGPlot::replotManual()
{
	m_arraySeriesData->lockMutex();
	setAxisScale(QwtPlot::yLeft,m_arraySeriesData->minimumY(), m_arraySeriesData->maximumY(), m_arraySeriesData->maximumY()/4);
	setAxisScale(QwtPlot::xBottom,m_arraySeriesData->minimumX(), m_arraySeriesData->maximumX(),1.0);
	replot();
	m_arraySeriesData->unLockMutex();
}

void EEGPlot::reset()
{
	m_arraySeriesData->reset();
	m_firstPacket = true;
}

void EEGPlot::refreshScale()
{
	m_arraySeriesData->refreshMinMax();
}

void EEGPlot::dataReceived(const CentData::AnalogData& data)
{
	drawSample(data);
}

void EEGPlot::drawSample(const CentData::AnalogData& data)
{
	if(m_numChannels == 0 || (data.num_channel < static_cast<int>(m_numChannels)) )
	{
		return;
	}
	qreal realSecond = 0.0;
	if(m_firstPacket)
	{
		m_firstPacket = false;
		m_redrawTimer.start();
	}
	else if(m_arraySeriesData->size() > 0)
	{
		realSecond = m_arraySeriesData->sample(m_arraySeriesData->size() -1).x() + SAMPLE_DELTA_REAL;
	}

	QPointF dataPoint(0,0);

	int idxBase = m_channelId * (data.num_channel/m_numChannels);
	for (int i=0; i < (data.num_channel/static_cast<int>(m_numChannels)) - 1; i++)
	{
		dataPoint.setX(realSecond);
		dataPoint.setY(data.channel[idxBase + i]);
		m_arraySeriesData->appendData(dataPoint);
		realSecond += SAMPLE_DELTA_REAL;
	}
}
