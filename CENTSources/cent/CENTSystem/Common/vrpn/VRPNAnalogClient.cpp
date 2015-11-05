#include "VRPNAnalogClient.h"

#include <QDebug>

class VRPNAnalogClientPrivate : public QObject
{
public:
	VRPNAnalogClientPrivate(bool onlyOnChanges, VRPNAnalogClient* parent)
		: QObject(parent)
		, m_parent(parent)
		, m_lastError(CENT::Success)
		, m_reportOnlyOnChange(onlyOnChanges)
	{}

	void analogCallback(vrpn_ANALOGCB analog);
	bool valuesChanged(const vrpn_ANALOGCB& analog);

	QString m_serverName;
	CENT::ErrorCode m_lastError;
	bool m_reportOnlyOnChange;
private:
	VRPNAnalogClient*    m_parent;
	CentData::AnalogData m_lastSent;
};

void VRPNAnalogClientPrivate::analogCallback(vrpn_ANALOGCB analog)
{
	if(m_reportOnlyOnChange)
	{
		if(!valuesChanged(analog))
		{
			return;
		}
	}

	m_lastSent.msg_time.tv_sec = analog.msg_time.tv_sec;
	m_lastSent.msg_time.tv_usec = analog.msg_time.tv_usec;
	m_lastSent.num_channel = analog.num_channel;

	if (CentData::CHANNEL_MAX != vrpn_CHANNEL_MAX)
	{
		qFatal("CentData::AnalogData is not compatible with VRPN version! %d, %d", CentData::CHANNEL_MAX, vrpn_CHANNEL_MAX);
	}

	for (int i = 0; i < m_lastSent.num_channel; ++i)
	{
		m_lastSent.channel[i] = analog.channel[i];
	}

	emit m_parent->dataReceived(m_lastSent);
}

bool VRPNAnalogClientPrivate::valuesChanged(const vrpn_ANALOGCB& analog)
{
	for(int i = 0; i < analog.num_channel; i++)
	{
		if(m_lastSent.channel[i] != analog.channel[i])
		{
			return true;
		}
	}
	return false;
}

void VRPN_CALLBACK vrpnAnalogCallback(void* user_data, vrpn_ANALOGCB analog)
{
	if(VRPNAnalogClientPrivate *client = static_cast<VRPNAnalogClientPrivate *>(user_data))
	{
		client->analogCallback(analog);
	}
}

VRPNAnalogClient::VRPNAnalogClient(const QString& serverName, bool onlyOnChanges, QObject* parent)
	: QObject(parent)
	, vrpn_Analog_Remote(serverName.toAscii().data())
	, d(new VRPNAnalogClientPrivate(onlyOnChanges, this))
{
	d->m_serverName = serverName;
	if(0 == register_change_handler( d, vrpnAnalogCallback ))
	{
		d->m_lastError = CENT::Success;
	}
	else
	{
		d->m_lastError = CENT::UnknownError;
	}
}

VRPNAnalogClient::~VRPNAnalogClient()
{
	unregister_change_handler(d, vrpnAnalogCallback );
}

const QString& VRPNAnalogClient::serverName() const
{
	return d->m_serverName;
}

CENT::ErrorCode VRPNAnalogClient::lastError()
{
	return d->m_lastError;
}
