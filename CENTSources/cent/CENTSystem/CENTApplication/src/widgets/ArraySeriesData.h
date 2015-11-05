#ifndef ARRAY_SERIES_DATA_H
#define ARRAY_SERIES_DATA_H
#include <qwt_series_data.h>
#include <QPointF>
#include <QMutex>

class ArraySeriesData: public QwtSeriesData<QPointF>
{
public:
	ArraySeriesData();
	virtual ~ArraySeriesData();

	// Get the sample at point i
	virtual QPointF sample(size_t i) const;

	// Get sample count
	virtual size_t size() const;

	// Get the bounding rect for whole data
	virtual QRectF boundingRect() const;

	// Append data thread safe
	void appendData(QPointF point);

	// Clear data thread safe
	void clearData();

	qreal minimumX();
	qreal maximumX();

	qreal minimumY();
	qreal maximumY();

	void lockMutex();
	void unLockMutex();

	// Clear data
	void reset();

	//min and max values need to be refreshed manually, because they are heavy operations
	void refreshMinMax();

private:
	QVector<QPointF> m_samples;
	QRectF m_boundingRect;
	QMutex m_mutex;

	qreal m_currMinX;
	qreal m_currMaxX;

	qreal m_currMinY;
	qreal m_currMaxY;

	void updateBounduingRect(const QPointF& point);
};
#endif //ARRAY_SERIES_DATA_H
