#include "NoisePlot.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QTime>

#include "qwt_color_map.h"
#include "qwt_thermo.h"

/*////////////////////////////
Noise channels descriptions (here indexed from 1)
1 = Current noise value
2 = Noise threshold1
3 = Noise threshold2 
4 = Min value
5 = Max value
////////////////////////////*/


namespace
{
	const int NOISE_PLOT_PIPE_WIDTH = 18;
	const double DEFAULT_MIN_VAL = 0.0;
	const double DEFAULT_MAX_VAL = 10.0;

	const double HALF_COLOR_BLUR_FRACTION = 0.02;

	const int CHANNEL_CURRENT_VALUE = 0;
	const int CHANNEL_THRESHOLD_ONE = 1;
	const int CHANNEL_THRESHOLD_TWO = 2;
	const int CHANNEL_MIN_VALUE     = 3;
	const int CHANNEL_MAX_VALUE     = 4;

	const QColor GOOD_SCALE_COLOR = Qt::green;
	const QColor MEDIUM_SCALE_COLOR = Qt::yellow;
	const QColor BAD_SCALE_COLOR = Qt::red;
}

NoisePlot::NoisePlot(const QString& label, QWidget* parent)
	: QWidget(parent)
{
	setObjectName("NoisePlot");
	createLayout(label);
}

NoisePlot::~NoisePlot()
{
}

void NoisePlot::onDataReceived(const CentData::AnalogData& data)
{
	double currentVal = data.channel[CHANNEL_CURRENT_VALUE];
	double thresholdOne = data.channel[CHANNEL_THRESHOLD_ONE];
	double thresholdTwo = data.channel[CHANNEL_THRESHOLD_TWO];
	double minVal = data.channel[CHANNEL_MIN_VALUE];
	double maxVal = data.channel[CHANNEL_MAX_VALUE];
	//qDebug() << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz") << __FUNCTION__ 
	//	<< "currentVal: "   << currentVal
	//	<< "thresholdOne: " << thresholdOne
	//	<< "thresholdTwo: " << thresholdTwo
	//	<< "minVal: "       << minVal
	//	<< "maxVal: "       << maxVal;

	if(minVal > thresholdOne || thresholdOne > thresholdTwo || thresholdTwo > maxVal)
	{
		return;
	}

	currentVal = currentVal < minVal ? minVal : currentVal;
	currentVal = currentVal > maxVal ? maxVal : currentVal;
	m_noisePlot->setRange(minVal, maxVal);

	QwtLinearColorMap* colorMap = new QwtLinearColorMap;
	colorMap->setMode(QwtLinearColorMap::ScaledColors);

	double extent = maxVal - minVal;
	thresholdOne -= minVal;
	thresholdTwo -= minVal;
	double blur = ((thresholdTwo - thresholdOne) / extent) >= (HALF_COLOR_BLUR_FRACTION * 2) ? HALF_COLOR_BLUR_FRACTION : 0;

	colorMap->addColorStop(0.0, GOOD_SCALE_COLOR);
	colorMap->addColorStop(thresholdOne / extent - blur, GOOD_SCALE_COLOR);
	colorMap->addColorStop(thresholdOne / extent + blur, MEDIUM_SCALE_COLOR);
	colorMap->addColorStop(thresholdTwo / extent - blur, MEDIUM_SCALE_COLOR);
	colorMap->addColorStop(thresholdTwo / extent + blur, BAD_SCALE_COLOR);
	colorMap->addColorStop(0.999, BAD_SCALE_COLOR);

	m_noisePlot->setColorMap(colorMap);
	m_noisePlot->setValue(currentVal);
	m_noisePlot->repaint();
}

void NoisePlot::onReset()
{
	m_noisePlot->setValue(m_noisePlot->minValue());
}

void NoisePlot::createLayout(const QString& labelText)
{
	QVBoxLayout* layout = new QVBoxLayout;

	// Create thermo plot
	m_noisePlot = new QwtThermo(this);
	m_noisePlot->setPipeWidth(NOISE_PLOT_PIPE_WIDTH);
	m_noisePlot->setScalePosition(QwtThermo::NoScale);

	QHBoxLayout* noiseLayout = new QHBoxLayout;
	noiseLayout->addStretch();
	noiseLayout->addWidget(m_noisePlot);
	noiseLayout->addStretch();

	QLabel* label = new QLabel(this);
	label->setText(labelText);
	label->setWordWrap(true);

	layout->addLayout(noiseLayout);
	layout->addWidget(label);

	setLayout(layout);
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
}
