#ifndef POWERBARHISTORYPLOT_H
#define POWERBARHISTORYPLOT_H

#include <QWidget>
#include "CentDataTypes.h"

class QwtPlot;
class QwtPlotCurve;
class QwtPointSeriesData;
class QwtText;
class ArraySeriesData;

class PowerBarHistoryPlot : public QWidget
{
	Q_OBJECT
public:
	explicit PowerBarHistoryPlot(double defaultTime = 60, double defaultPowerLevel = 10, QWidget *parent = 0);

public slots:
	void setTimeAxis(double maxTime);
	void setPowerRange(double minTheta, double maxTheta, double minBeta, double maxBeta);
	void setColors(const Qt::GlobalColor& powerThetaColor, const Qt::GlobalColor& powerBetaColor);
	void setValue(double thetaValue, double betaValue, CentData::time time);
	void reset();

private:
	bool setRange(double min, double max);
	QPointF makePointF(qreal x, qreal y);
	void setupCurves();
	QwtPlotCurve* createCurve(const QwtText& title, const QPen& pen, ArraySeriesData* data);

private:
	QwtPlot* m_plot;
	QwtPlotCurve* m_curve1;
	QwtPlotCurve* m_curve2;

	ArraySeriesData* m_series1;
	ArraySeriesData* m_series2;

	double m_overalTime;
	double m_lastSampleTime;

	bool m_firstTime;
	CentData::time m_startTime;

	double m_defaultTime;
	double m_defaultPowerLevel;
	double m_powerAxisMax;
	double m_powerAxisMin;
	double m_timeAxisMax;
};

#endif // POWERBARHISTORYPLOT_H
