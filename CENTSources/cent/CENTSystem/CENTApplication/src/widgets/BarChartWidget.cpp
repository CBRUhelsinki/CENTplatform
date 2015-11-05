#include "BarChartWidget.h"

#include <QVBoxLayout>
#include <QList>
#include <QPair>
#include <QPen>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>

#include "BarChartItem.h"

namespace BarChartPrivate
{
	//this is a workaround, to be fixed in the future
	const int LETTER_WIDTH = 7; // works only with default font
	const int BARS_LIMIT   = 40; //still looks ok on smaller resolutions

	class BarChartScaleDraw : public QwtScaleDraw
	{
	public:
		BarChartScaleDraw()
			: QwtScaleDraw()
			, m_extent(0) {}
		virtual ~BarChartScaleDraw() {}

		QwtText label( double value) const
		{
			int val = value;
			if(val % 2 == 0 || val != value)
			{
				return QwtText();
			}

			val /= 2;
			if(val >= 0 && val < m_data.count())
			{
				int max_letters = (m_extent / LETTER_WIDTH);
				int num_letters = m_data.value(val).first.length();
				QString postfix;
				if(num_letters > max_letters)
				{
					num_letters = max_letters - 1; // -1 to make room for "..."
					postfix = QString("...");
				}
				return QwtText(m_data.value(val).first.left(num_letters) + postfix);
			}
			else
			{
				return QwtText();
			}
		}

		void setData(const QList< QPair<QString, double> > &data)
		{
			m_data = QList< QPair<QString, double> >(data);
		}

		virtual double extent( const QFont & font) const
		{
			Q_UNUSED(font)
			return m_extent;
		}

		void setExtent(double ex)
		{
			m_extent = ex;
		}

	private:
		QList< QPair<QString, double> > m_data;
		double m_extent;
	};
}

BarChartWidget::BarChartWidget(const QString& title, QWidget* parent)
	: QWidget(parent)
	, m_barChartItem(0)
	, m_plot(0)
	, m_scaleDraw(0)
{
	createLayout(title);
}

BarChartWidget::~BarChartWidget()
{
}

void BarChartWidget::setData(const QList< QPair<QString, double> > &data)
{
	m_plot->updateLayout();
	Q_ASSERT(m_barChartItem != 0);
	Q_ASSERT(m_scaleDraw != 0);

	QList< QPair<QString, double> > myData(data);
	while(myData.count() > BarChartPrivate::BARS_LIMIT)
	{
		myData.removeFirst();
	}
	m_scaleDraw->setData(myData);
	m_barChartItem->setData(myData);
}

void BarChartWidget::setAxisLabel(int axisId, const QString& str)
{
	QwtText txt(str);
	m_plot->setAxisTitle(axisId, txt);
}

void BarChartWidget::resizeEvent(QResizeEvent *event)
{
	m_barChartItem->refreshXAxis();
	QWidget::resizeEvent(event);
}

void BarChartWidget::showEvent(QShowEvent *event)
{
	m_barChartItem->refreshXAxis();
	QWidget::showEvent(event);
}

void BarChartWidget::createLayout(const QString& title)
{
	m_barChartItem = new BarChartItem();
	m_plot = new QwtPlot();
	m_scaleDraw = new BarChartPrivate::BarChartScaleDraw();

	m_plot->setCanvasBackground(QColor(Qt::white));
	m_plot->setTitle(title);

	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableX(false);
	grid->enableYMin(true);
	grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::transparent, 0 , Qt::NoPen));
	grid->attach(m_plot);

	m_barChartItem->attach(m_plot);

	//setup X axis scale
	m_plot->setAxisScaleDraw(QwtPlot::xBottom, m_scaleDraw);

	m_plot->setAxisLabelRotation(QwtPlot::xBottom, 270.0 );
	m_scaleDraw->setSpacing(60.0);
	m_scaleDraw->setExtent(115.0);

	m_plot->axisScaleDraw(QwtPlot::xBottom)->enableComponent(QwtScaleDraw::Backbone, false);
	m_plot->axisScaleDraw(QwtPlot::xBottom)->enableComponent(QwtScaleDraw::Ticks, false);
	m_plot->axisScaleDraw(QwtPlot::xBottom)->enableComponent(QwtScaleDraw::Labels, true);

	//setup Y axis scale
	m_plot->setAxisScale(QwtPlot::yLeft, 0.0, 100.0, 20);
	m_plot->setAutoReplot(true);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(m_plot);
	setLayout(layout);
}
