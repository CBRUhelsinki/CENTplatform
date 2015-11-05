#include "VRPNClientThread.h"

#include "Connect.h"

VRPNClientThread::VRPNClientThread(QObject* parent)
	: QThread(parent)
	, m_isRunning(true)
{
	qRegisterMetaType<CentData::AnalogData>();
	qRegisterMetaType<CentData::DigitalData>();
	setObjectName("VRPNClientThread");
}

VRPNClientThread::~VRPNClientThread()
{
	stop();
	wait();
}

CENT::ErrorCode VRPNClientThread::registerDigitalClient(const QString& serverName, const QObject* receiver, const char* method)
{
	CENT::ErrorCode ret = CENT::Success;
	foreach(const VRPNDigitalClient* digitalClient, m_digitalClients)
	{
		if(digitalClient->serverName() == serverName)
		{
			CENT::connect(digitalClient, SIGNAL(dataReceived(CentData::DigitalData)), receiver, method);
			return ret;
		}
	}

	VRPNDigitalClient* newDigitalClient = new VRPNDigitalClient(serverName, this);
	ret = newDigitalClient->lastError();
	if(ret != CENT::Success)
	{
		delete newDigitalClient;
		newDigitalClient = NULL;
		return ret;
	}
	//newDigitalClient->moveToThread(this);	//TODO: whole VRPNClientThread class should be rewritten to follow Qt guidelines regarding QThread
	CENT::connect(newDigitalClient, SIGNAL(dataReceived(CentData::DigitalData)), receiver, method);
	m_digitalClients.push_back(newDigitalClient);
	return ret;
}

CENT::ErrorCode VRPNClientThread::registerAnalogClient(const QString& serverName, const QObject* receiver, const char* method, bool reportOnlyOnChanges)
{
	CENT::ErrorCode ret = CENT::Success;
	foreach(const VRPNAnalogClient* analogClient, m_analogClients)
	{
		if(analogClient->serverName() == serverName)
		{
			CENT::connect(analogClient, SIGNAL(dataReceived(CentData::AnalogData)), receiver, method);
			return ret;
		}
	}

	VRPNAnalogClient* newAnalogClient = new VRPNAnalogClient(serverName, reportOnlyOnChanges, this);
	ret = newAnalogClient->lastError();
	if(ret != CENT::Success)
	{
		delete newAnalogClient;
		newAnalogClient = NULL;
		return ret;
	}
	//newAnalogClient->moveToThread(this);	//TODO: whole VRPNClientThread class should be rewritten to follow Qt guidelines regarding QThread
	CENT::connect(newAnalogClient, SIGNAL(dataReceived(CentData::AnalogData)), receiver, method);
	m_analogClients.push_back(newAnalogClient);
	return ret;
}

void VRPNClientThread::stop()
{
	m_isRunning = false;
}

void VRPNClientThread::run()
{
	//vrpn_BaseClass* activeClient = 0;

	//if(! m_analogClients.isEmpty())
	//{
	//	activeClient = m_analogClients.first();
	//}
	//else if(!m_digitalClients.isEmpty())
	//{
	//	activeClient = m_digitalClients.first();
	//}

	while(m_isRunning /*&& activeClient*/)
	{
		foreach(vrpn_BaseClass* activeClient, m_analogClients)
		{
			activeClient->mainloop();
		}
		foreach(vrpn_BaseClass* activeClient, m_digitalClients)
		{
			activeClient->mainloop();
		}
		msleep(20);
	}
	m_isRunning = true;
}

