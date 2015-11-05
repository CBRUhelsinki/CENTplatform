#include "powerlevelbar.h"

#include <QApplication>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>

#include "qwt_thermo.h"

PowerLevelBar::PowerLevelBar(int pipeWidth, QWidget* parent)
	: QWidget(parent)
{
	m_thermoPositive = createQwtThermo(pipeWidth, 0, 5);
	m_thermoNegative = createQwtThermo(pipeWidth, 0, -5);

	setColors(Qt::green, Qt::green);

	m_thermoPositive->setRangeFlags(QwtInterval::ExcludeMinimum);
	m_thermoNegative->setRangeFlags(QwtInterval::ExcludeMinimum);

	QLabel* label = new QLabel;
	label->setFrameStyle(QFrame::HLine | QFrame::Plain);
	label->setLineWidth(1);
	label->setMaximumHeight(1);
	label->setMargin(0);

	QVBoxLayout* thermoLayout = new QVBoxLayout;
	thermoLayout->addWidget(m_thermoPositive);
	thermoLayout->addWidget(label);
	thermoLayout->addWidget(m_thermoNegative);
	thermoLayout->setSpacing(0);

	setLayout(thermoLayout);
	setValue(0.0);
}

void PowerLevelBar::setPipeWidth(int w)
{
	m_thermoPositive->setPipeWidth(w);
	m_thermoNegative->setPipeWidth(w);
}

void PowerLevelBar::setColors(const QColor& negative, const QColor& positive)
{
	QPalette palette = QApplication::palette();
	palette.setColor(QPalette::ButtonText, positive);
	m_thermoPositive->setPalette(palette);
	palette.setColor(QPalette::ButtonText, negative);
	m_thermoNegative->setPalette(palette);
}

void PowerLevelBar::setRange(double min, double max)
{
	if(min >= max || min > 0 || max < 0)
	{
		return;
	}

	if(min == 0)
	{
		m_thermoNegative->setVisible(false);
	}
	else
	{
		m_thermoNegative->setRange(0, min);
		m_thermoNegative->setVisible(true);
	}

	if(max == 0)
	{
		m_thermoPositive->setVisible(false);
	}
	else
	{
		m_thermoPositive->setRange(0, max);
		m_thermoPositive->setVisible(true);
	}
}

void PowerLevelBar::setValue(double val)
{
	if(val >= 0)
	{
		m_thermoPositive->setValue(val);
		m_thermoNegative->setValue(m_thermoNegative->maxValue());
	}
	else
	{
		val = m_thermoNegative->maxValue() - val;
		m_thermoPositive->setValue(0);
		m_thermoNegative->setValue(val);
	}
}

QwtThermo* PowerLevelBar::createQwtThermo(int pipeWidth, double min, double max)
{
	QwtThermo* thermo = new QwtThermo();
	thermo->setBorderWidth(0);
	thermo->setSpacing(0);
	thermo->setScalePosition(QwtThermo::NoScale);
	thermo->setPipeWidth(pipeWidth);
	thermo->setRange(min, max);
	return thermo;
}
