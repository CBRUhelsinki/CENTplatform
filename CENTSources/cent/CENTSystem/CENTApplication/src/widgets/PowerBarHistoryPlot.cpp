#include "PowerBarHistoryPlot.h"

#include <QHBoxLayout>
#include <QLabel>

#include "ArraySeriesData.h"

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>

namespace
{
	const qreal PEN_WIDTH = 2.0;
	const qreal PLOT_ODDS = 0.01;

	const char* THETA_NAME      = "Power levels of Theta";
	const char* BETA_NAME       = "Power levels of Beta";
	const char* TIME_AXIS_NAME  = "Time sec";
	const char* POWER_AXIS_NAME = "Power";
}

PowerBarHistoryPlot::PowerBarHistoryPlot(double defaultTime, double defaultPowerLevel, QWidget* parent)
	: QWidget(parent)
	, m_overalTime(0.0)
	, m_lastSampleTime(0.0)
	, m_firstTime(true)
	, m_defaultTime(defaultTime)
	, m_defaultPowerLevel(defaultPowerLevel)
	, m_powerAxisMax(defaultPowerLevel)
	, m_powerAxisMin(-1*defaultPowerLevel)
	, m_timeAxisMax(defaultTime)
{
	m_plot = new QwtPlot();
	m_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_plot->setAxisScale(QwtPlot::xBottom, 0, defaultTime, defaultTime/20.0);
	m_plot->setAxisScale(QwtPlot::yLeft, -1 * defaultPowerLevel, defaultPowerLevel, defaultPowerLevel/2.0);
	m_plot->setAxisAutoScale(QwtPlot::xBottom, false);
	m_plot->setCanvasBackground(QBrush(Qt::white));
	m_plot->setAxisTitle(QwtPlot::xBottom, tr(TIME_AXIS_NAME));
	m_plot->setAxisTitle(QwtPlot::yLeft, tr(POWER_AXIS_NAME));
	
	// legend
	QwtLegend *legend = new QwtLegend;
	legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
	m_plot->insertLegend(legend, QwtPlot::BottomLegend);

	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableX(false);
	grid->enableYMin(true);
	grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::transparent, 0 , Qt::NoPen));
	grid->attach(m_plot);

	setupCurves();

	QHBoxLayout* historyPlotlayout = new QHBoxLayout;
	historyPlotlayout->addWidget(m_plot);
	historyPlotlayout->setSpacing(0);

	setColors(Qt::blue, Qt::red);

	setLayout(historyPlotlayout);
}

void PowerBarHistoryPlot::reset()
{
	m_plot->setAxisAutoScale(QwtPlot::xBottom, false);
	m_overalTime = 0.0;
	m_lastSampleTime = 0.0;
	m_firstTime = true;
	m_startTime.tv_sec = 0;
	m_startTime.tv_usec = 0;
	m_series1->reset();
	m_series2->reset();
	m_powerAxisMax = m_defaultPowerLevel;
	m_powerAxisMin = -1 * m_defaultPowerLevel;
	m_plot->replot();
}

void PowerBarHistoryPlot::setTimeAxis(double maxTime)
{
	if(maxTime < 0)
	{
		// needed when we change the game parameters, time checkbox disabled
		m_timeAxisMax = m_defaultTime;
	}
	else
	{
		m_timeAxisMax = maxTime;
	}

	m_plot->setAxisScale(QwtPlot::xBottom, 0, m_timeAxisMax, m_timeAxisMax/20.0);
	m_plot->replot();
}

bool PowerBarHistoryPlot::setRange(double min, double max)
{
	bool setNewRange = false;
	if(min >= max || min > 0 || max < 0)
	{
		return false;
	}

	if(m_powerAxisMin > min)
	{
		m_powerAxisMin = min;
		setNewRange = true;
	}
	
	if(m_powerAxisMax < max)
	{
		m_powerAxisMax = max;
		setNewRange = true;
	}
	return setNewRange;
}

void PowerBarHistoryPlot::setPowerRange(double minTheta, double maxTheta, double minBeta, double maxBeta)
{
	if(setRange(minTheta, maxTheta) || setRange(minBeta, maxBeta))
	{
		m_plot->setAxisScale(QwtPlot::yLeft, m_powerAxisMin, m_powerAxisMax, m_powerAxisMax/2);
		m_plot->replot();
	}
}

void PowerBarHistoryPlot::setColors(const Qt::GlobalColor& powerThetaColor, const Qt::GlobalColor& powerBetaColor)
{
	m_curve1->setPen(QPen(QBrush(powerThetaColor), PEN_WIDTH));
	m_curve2->setPen(QPen(QBrush(powerBetaColor), PEN_WIDTH, Qt::DotLine));
}

void PowerBarHistoryPlot::setValue(double thetaValue, double betaValue, CentData::time time)
{
	if(m_firstTime)
	{
		m_startTime = time;
		m_firstTime = false;
		// we need to start from (0,0) point
		m_series1->appendData(QPointF(0.0, 0.0));
		m_series2->appendData(QPointF(0.0, 0.0));
	}

	double fraction = static_cast<double>((time.tv_usec - m_startTime.tv_usec) / 1000);
	fraction /= 1000.0;
	m_overalTime = time.tv_sec - m_startTime.tv_sec + fraction;
	if(m_overalTime - m_lastSampleTime < PLOT_ODDS)
	{
		return;
	}
	m_lastSampleTime = m_overalTime;

	if(m_overalTime > m_timeAxisMax)
	{
		m_plot->setAxisAutoScale(QwtPlot::xBottom, true);
	}

	m_series1->appendData(makePointF(m_overalTime, thetaValue));
	m_series2->appendData(makePointF(m_overalTime, betaValue));

	m_plot->replot();
}

void PowerBarHistoryPlot::setupCurves()
{
	m_series1 = new ArraySeriesData();
	m_series2 = new ArraySeriesData();

	m_curve1 = createCurve(QwtText(tr(THETA_NAME)), QPen(QBrush(Qt::blue), PEN_WIDTH), m_series1);
	m_curve2 = createCurve(QwtText(tr(BETA_NAME)), QPen(QBrush(Qt::red), PEN_WIDTH, Qt::DotLine), m_series2);
}

QwtPlotCurve* PowerBarHistoryPlot::createCurve(const QwtText& title, const QPen& pen, ArraySeriesData* data)
{
	QwtPlotCurve* curve = new QwtPlotCurve(title);
	curve->setStyle(QwtPlotCurve::Lines);
	curve->setPen(pen);

	curve->setRenderHint(QwtPlotItem::RenderAntialiased, false);
	curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, true);

	curve->setData(data);
	curve->attach(m_plot);

	return curve;
}

QPointF PowerBarHistoryPlot::makePointF(qreal x, qreal y)
{
	if (y > m_powerAxisMax)
	{
		y = m_powerAxisMax;
	}
	else if(y < m_powerAxisMin)
	{
		y = m_powerAxisMin;
	}

	return QPointF(x, y);
}
