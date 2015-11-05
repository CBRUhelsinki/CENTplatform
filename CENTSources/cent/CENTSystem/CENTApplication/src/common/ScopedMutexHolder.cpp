#include "ScopedMutexHolder.h"
#include <QApplication>
#include <QMutex>
ScopedMutexHolder::ScopedMutexHolder(QMutex* mutex)
	: m_mutex(mutex)
{
	while (! m_mutex->tryLock())
	{
		qApp->processEvents(QEventLoop::AllEvents,5);
	}
}

ScopedMutexHolder::~ScopedMutexHolder()
{
	m_mutex->unlock();
}