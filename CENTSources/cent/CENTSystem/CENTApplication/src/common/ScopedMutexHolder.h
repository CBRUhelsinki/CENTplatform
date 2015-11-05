#ifndef SCOPED_MUTEX_HOLDER_H
#define SCOPED_MUTEX_HOLDER_H

#include <QMutex>

class ScopedMutexHolder
{
public:
	explicit ScopedMutexHolder(QMutex* mutex);
	virtual ~ScopedMutexHolder();

	QMutex* m_mutex;
};
#endif //SCOPED_MUTEX_HOLDER_H