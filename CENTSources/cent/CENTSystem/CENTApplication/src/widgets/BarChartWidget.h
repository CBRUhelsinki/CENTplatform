#ifndef BAR_CHART_WIDGET_H
#define BAR_CHART_WIDGET_H

#include <QWidget>

class BarChartItem;
class QwtPlot;

namespace BarChartPrivate
{
	class BarChartScaleDraw;
}

class BarChartWidget : public QWidget
{
	Q_OBJECT
public:
	enum Axis // from qwt_plot.h
	{
		LEFT_AXIS_ID = 0,
		RIGHT_AXIS_ID,
		BOTTOM_AXIS_ID,
		TOP_AXIS_ID
	};

	explicit BarChartWidget(const QString& title = QString(), QWidget* parent = 0);
	virtual ~BarChartWidget();

	void setData(const QList< QPair<QString, double> > &data);
	void setAxisLabel(int axisId, const QString& text);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void showEvent(QShowEvent *event);

private:
	void createLayout(const QString& title);

private:
	BarChartItem*      m_barChartItem;
	QwtPlot*           m_plot;

	BarChartPrivate::BarChartScaleDraw* m_scaleDraw;
};
#endif // BAR_CHART_WIDGET_H
