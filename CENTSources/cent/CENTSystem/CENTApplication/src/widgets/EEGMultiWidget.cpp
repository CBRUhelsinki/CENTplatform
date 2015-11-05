#include "EEGMultiWidget.h"
#include "EEGPlot.h"
#include "connect.h"

#include <QDateTime>
#include <QVBoxLayout>
#include <QMap>
#include "qwt.h"

namespace
{
	// Range values
	const int MAX_VALUE_FOR_CHANNEL = 66000;
	const int MIN_VALUE_FOR_CHANNEL = 0;

	template <class Key, class T>
	inline QMap<Key, T> &operator<< (QMap<Key, T>& a_map, const QPair<Key, T>& a_pair)
	{
		a_map.insert(a_pair.first, a_pair.second);
		return a_map;
	}
}

EEGMultiWidget::EEGMultiWidget(QWidget *parent)
	: QWidget(parent)
	, m_layout(new QVBoxLayout(this))
{
	
	setLayout(m_layout);
}

EEGMultiWidget::~EEGMultiWidget()
{
}

void EEGMultiWidget::configureLayout() // temporary workaround to poor design, should be called after adding all plots
{
	for(int i = 0; i < m_layout->children().count() - 1; i++)
	{
		m_layout->setStretch(i, 84);
	}
	m_layout->setStretch(m_layout->children().count() - 1, 110);
}

void EEGMultiWidget::onReset()
{
	emit resetPlots();
}

void EEGMultiWidget::onDataReceived(const CentData::AnalogData& data)
{
//	qDebug() << __FUNCTION__ << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz");
	emit dataReceived(data);
}

void EEGMultiWidget::onRefreshScales()
{
	emit refreshScales();
}

bool EEGMultiWidget::addPlot(unsigned int chNum, unsigned int numChannels, bool showXScale)
{
	EEGPlot* newPlot = new EEGPlot(chNum, this, this, SIGNAL(dataReceived(CentData::AnalogData)), numChannels);

	newPlot->enableAxis(QwtPlot::xBottom, showXScale);

	m_layout->addWidget(newPlot);
	CENT::connect(this, SIGNAL(resetPlots()), newPlot, SLOT(reset()));
	CENT::connect(this, SIGNAL(refreshScales()), newPlot, SLOT(refreshScale()));
	return true;
}
