#include "VRPNDigitalClient.h"
#include <tchar.h>
#include <windows.h>
#include <qdebug.h>

class VRPNDigitalClientPrivate : public QObject
{
public:
	VRPNDigitalClientPrivate(VRPNDigitalClient* parent)
		: QObject(parent)
		, m_parent(parent)
		, m_lastError(CENT::Success)
	{
		QueryPerformanceCounter(&m_startTime);
		QueryPerformanceFrequency((LARGE_INTEGER *)&m_freq);
	}

	void digitalCallback(vrpn_BUTTONCB button);
	void createFakeTimestamp(vrpn_BUTTONCB& button);
	long calculateElapsedTime();
	void calculateSecondsPart(long elapsedTime);
	void calculateMicrosecondsPart(long elapsedTime);
public:
	QString m_serverName;
	CENT::ErrorCode m_lastError;
	LARGE_INTEGER m_startTime;	
	LARGE_INTEGER m_freq;

private:
	VRPNDigitalClient* m_parent;
};

long VRPNDigitalClientPrivate::calculateElapsedTime()
{
  if (m_freq.QuadPart == 0)
  {
	return -1;
  }
  else
  {
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);

	return static_cast<long>( (c.QuadPart - m_startTime.QuadPart) * 1000000/ m_freq.QuadPart );
  }
}

void VRPNDigitalClientPrivate::createFakeTimestamp(vrpn_BUTTONCB& button)
{//<TODO> tempolary code
	LARGE_INTEGER ctr1;

	QueryPerformanceCounter((LARGE_INTEGER *)&ctr1);

	long diff = calculateElapsedTime();
	long seconds = floor((diff/1000000.0));
	long microseconds = diff - seconds*1000000.0;
	button.msg_time.tv_sec = seconds;
	button.msg_time.tv_usec = microseconds;
	qDebug() << "timestamp:" << seconds << ", " << microseconds;

}

void VRPNDigitalClientPrivate::digitalCallback(vrpn_BUTTONCB button)
{
	createFakeTimestamp(button);

	CentData::DigitalData data;
	data.button = button.button;
	data.state = button.state;
	data.msg_time.tv_sec = button.msg_time.tv_sec;
	data.msg_time.tv_usec = button.msg_time.tv_usec;

	emit m_parent->dataReceived(data);
}

void VRPN_CALLBACK vrpnButtonCallback(void* user_data, vrpn_BUTTONCB button)
{
	if(VRPNDigitalClientPrivate *client = static_cast<VRPNDigitalClientPrivate *>(user_data))
	{

		client->digitalCallback(button);
	}
}

VRPNDigitalClient::VRPNDigitalClient(const QString& serverName, QObject* parent)
	: QObject(parent)
	, vrpn_Button_Remote(serverName.toAscii().data())
	, d(new VRPNDigitalClientPrivate(this))
{
	d->m_serverName = serverName;
	if(0 == register_change_handler( d, vrpnButtonCallback ))
	{
		d->m_lastError = CENT::Success;
	}
	else
	{
		d->m_lastError = CENT::UnknownError;
	}
}

VRPNDigitalClient::~VRPNDigitalClient()
{
	unregister_change_handler(d, vrpnButtonCallback );
}

const QString& VRPNDigitalClient::serverName() const
{
	return d->m_serverName;
}

CENT::ErrorCode VRPNDigitalClient::lastError()
{
	return d->m_lastError;
}

