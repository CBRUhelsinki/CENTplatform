#ifndef BAR_CHART_ITEM_H
#define BAR_CHART_ITEM_H

#include <qwt_plot_item.h>

#include <QList>
#include <QPair>

class BarChartItem : public QwtPlotItem
{
public:
	BarChartItem(const QwtText &title = QwtText());

	void setData(const QList< QPair<QString, double> > &data);
	const QList< QPair<QString, double> > &data(void) const;

	virtual void draw( QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect ) const;

	void refreshXAxis();
private:
	QList< QPair<QString, double> > m_data;
};

#endif // BAR_CHART_ITEM_H
