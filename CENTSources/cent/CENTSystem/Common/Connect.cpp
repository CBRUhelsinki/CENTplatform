#include "Connect.h"

namespace CENT
{
	bool connect(const QObject *sender, const char *signal,
					 const QObject *receiver, const char *member,
					 Qt::ConnectionType type)
	{
		if (! QObject::connect(sender, signal, receiver, member, type))
		{
			Q_ASSERT_X(false, "centConnect", "connection not made");
			return false;
		}
		return true;
	}

	bool disconnect(const QObject *sender, const char *signal,
						const QObject *receiver, const char *member)
	{
		if (! QObject::disconnect(sender, signal, receiver, member))
		{
			Q_ASSERT_X(false, "centDisconnect", "disconnection not made");
			return false;
		}
		return true;
	}
}