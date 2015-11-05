#include "ArraySeriesData.h"
#include "ScopedMutexHolder.h"

#include <QApplication>
#include <float.h>
namespace
{
	const size_t MAX_HOLDED_PACKETS = 1000;
}

ArraySeriesData::ArraySeriesData()
	: m_currMinY(DBL_MAX)
	, m_currMaxY(0.0f)
{
}

ArraySeriesData::~ArraySeriesData()
{
}

QPointF ArraySeriesData::sample(size_t i) const
{
	return m_samples[i];
}

size_t ArraySeriesData::size() const
{
	return m_samples.size();
}

QRectF ArraySeriesData::boundingRect() const
{
	return m_boundingRect;
}

void ArraySeriesData::updateBounduingRect(const QPointF& point)
{
	Q_UNUSED(point);
}

qreal ArraySeriesData::minimumX()
{
	return m_currMinX;
}

qreal ArraySeriesData::maximumX()
{
	return m_currMaxX;
}

qreal ArraySeriesData::minimumY()
{
	return m_currMinY;
}

qreal ArraySeriesData::maximumY()
{
	return m_currMaxY;
}

void ArraySeriesData::lockMutex()
{
	while(!m_mutex.tryLock())
	{
		qApp->processEvents(QEventLoop::AllEvents,10);
	}
}

void ArraySeriesData::unLockMutex()
{
	m_mutex.unlock();
}

void ArraySeriesData::reset()
{
	lockMutex();

	m_samples.clear();

	unLockMutex();
}

void ArraySeriesData::refreshMinMax()
{
	qreal yMax = std::numeric_limits<qreal>::min();
	qreal yMin = std::numeric_limits<qreal>::max();
	qreal xMax = std::numeric_limits<qreal>::min();
	qreal xMin = std::numeric_limits<qreal>::max();

	foreach (QPointF p, m_samples)
	{
		yMin = qMin(yMin, p.y());
		yMax = qMax(yMax, p.y());
		xMin = qMin(xMin, p.x());
		xMax = qMax(xMax, p.x());
	}

	m_currMinY = yMin;
	m_currMaxY = yMax;
	m_currMinX = xMin;
	m_currMaxX = xMax;
}

void ArraySeriesData::appendData(QPointF point)
{
	lockMutex();

	updateBounduingRect(point);

	if (m_samples.size() > MAX_HOLDED_PACKETS)
	{
		m_samples.remove(0);
		m_currMinX = m_samples[0].x();
	}
	else if (m_samples.isEmpty())
	{
		m_currMinX = point.x();
	}

	if(m_currMinY > point.y())
	{
		m_currMinY = point.y();
	}

	if(m_currMaxY < point.y())
	{
		m_currMaxY = point.y();
	}

	m_currMaxX = point.x();
	m_samples.append(point);

	m_boundingRect.setRect(m_currMinX, m_currMinY, m_currMaxX, m_currMaxY);

	unLockMutex();
}