#ifndef CENT_CONNECT_H
#define CENT_CONNECT_H

#include <QObject>

namespace CENT
{
	bool connect(const QObject *sender, const char *signal,
                 const QObject *receiver, const char *member,
				 Qt::ConnectionType type = Qt::AutoConnection);

	bool disconnect(const QObject *sender, const char *signal,
                    const QObject *receiver, const char *member);
}

#endif

