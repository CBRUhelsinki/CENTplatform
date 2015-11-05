#ifndef VRPN_CLIENT_THREAD_H
#define VRPN_CLIENT_THREAD_H

#include <QThread>
#include <QVector>

class AnalogClient;
class DigitalClient;

#include "VRPNDigitalClient.h"
#include "VRPNAnalogClient.h"

class VRPNClientThread : public QThread
{
public:
	explicit VRPNClientThread(QObject* parent = 0);
	virtual ~VRPNClientThread();

	CENT::ErrorCode registerDigitalClient(const QString& serverName, const QObject* receiver, const char* method);
	CENT::ErrorCode registerAnalogClient(const QString& serverName, const QObject* receiver, const char* method, bool reportOnlyOnChanges = false);

	virtual void stop();
protected:
	virtual void run();

private:
	volatile bool m_isRunning;

	QVector<VRPNAnalogClient*>   m_analogClients;
	QVector<VRPNDigitalClient*>  m_digitalClients;
};


#endif // VRPN_CLIENT_THREAD_H
