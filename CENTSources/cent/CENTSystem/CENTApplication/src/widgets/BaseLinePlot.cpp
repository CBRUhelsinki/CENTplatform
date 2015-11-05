#include "BaseLinePlot.h"

#include "Connect.h"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_engine.h>

namespace
{
	const QString X_AXIS_TEXT = "Frequency Hz";

	const QString Y_AXIS_RICHTEXT = "Power log(µV<sup>2</sup>)";

	const int X_SCALE_MIN = 0;
	const int X_SCALE_MAX = 80;
}

BaseLinePlot::BaseLinePlot(const QString& plotName, QWidget* parent)
	: QWidget(parent)
{
	createLayout(plotName);
}

BaseLinePlot::~BaseLinePlot()
{
}

void BaseLinePlot::setData(const double* dataX, const double* dataY, int dataSize)
{
	m_plotCurve->setRawSamples(dataX, dataY, dataSize);
	m_plot->replot();
}

void BaseLinePlot::createLayout(const QString& plotName)
{
	// Create plot
	m_plot = new QwtPlot(plotName);

	m_plot->setCanvasBackground(QBrush(Qt::white));

	// Setup axes
	QwtText xText(X_AXIS_TEXT);
	m_plot->setAxisTitle(QwtPlot::xBottom, xText);
	QwtText yText(Y_AXIS_RICHTEXT, QwtText::RichText);
	m_plot->setAxisTitle(QwtPlot::yLeft, yText);

	// not using logarithmic scale engine as it behaves strange if the range is smaller than 10
	m_plot->setAxisScale(QwtPlot::xBottom, X_SCALE_MIN, X_SCALE_MAX);
	m_plot->setAxisAutoScale(QwtPlot::yLeft);

	m_plotCurve = new QwtPlotCurve();
	m_plotCurve->attach(m_plot);

	m_plotCurve->setPen(QColor(Qt::black));
	m_plotCurve->setBrush(Qt::transparent);

	m_plotCurve->setRenderHint(QwtPlotItem::RenderAntialiased);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(m_plot);
	setLayout(mainLayout);
}
