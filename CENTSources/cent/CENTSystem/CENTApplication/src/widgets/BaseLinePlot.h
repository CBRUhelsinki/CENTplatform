#ifndef BASE_LINE_PLOT_H
#define BASE_LINE_PLOT_H

#include <QWidget>
#include <QPointF>
#include <QMap>

class QwtPlot;
class QwtPlotCurve;
class QwtPlotItem;

class BaseLinePlot: public QWidget
{
	Q_OBJECT
public:
	explicit BaseLinePlot(const QString& plotName = "", QWidget* parent = 0);
	virtual ~BaseLinePlot();

public:
	void setData(const double* dataX, const double* dataY,int dataSize);

private:
	void createLayout(const QString& plotName);

private:
	QwtPlot*      m_plot;
	QwtPlotCurve* m_plotCurve;
};

#endif // BASE_LINE_PLOT_H