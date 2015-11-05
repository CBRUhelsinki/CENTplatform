#include "BarChartItem.h"

#include <QPainter>
#include <QBrush>

#include "qwt_plot.h"
#include "qwt_scale_div.h"

BarChartItem::BarChartItem(const QwtText &title)
	: QwtPlotItem(title)
{
}

void BarChartItem::setData(const QList< QPair<QString, double> > &data)
{
	m_data = QList< QPair<QString, double> >(data);

	QList< QPair<QString, double> >::const_iterator it;
	for(it = m_data.begin(); it != m_data.end(); ++it)
	{
		qDebug() << it->first << ", " << it->second;
	}
	// Set X axis
	refreshXAxis();
}

void BarChartItem::refreshXAxis()
{
	//causes recalculate scale labels
	double ext = plot()->axisScaleDiv(QwtPlot::xBottom)->upperBound() == (m_data.size()*2) ? -0.01 : 0.0;
	plot()->setAxisScale(QwtPlot::xBottom, 0, (m_data.size()*2) + ext, 1.0);
}

const QList< QPair<QString, double> > &BarChartItem::data(void) const
{
	return m_data;
}

void BarChartItem::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect ) const
{
	Q_UNUSED(canvasRect)
	QwtScaleMap myXMap(xMap);
	myXMap.setScaleInterval(0, (m_data.size()*2));

	for(int i = 0;i < m_data.size();++i)
	{
		double halfWidth = (myXMap.transform(1) - myXMap.transform(0) )/ 2;
		// Paint the bar
		double x = myXMap.transform(i*2) + halfWidth * 0.4;
		double y = yMap.transform(0);
		double w = myXMap.transform((i*2)+1) - myXMap.transform(i*2);
		w *= 1.6;   // to have smaller gaps between bars
		double value = m_data[i].second;
		double h = yMap.transform(value) - yMap.transform(0);
		painter->fillRect(x, y, w, h, QBrush(Qt::blue));
	}
}
