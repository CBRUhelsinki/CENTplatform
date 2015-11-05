#ifndef VRPN_SERVER_THREAD_H
#define VRPN_SERVER_THREAD_H

#include <QThread>

class QTimer;
class myButtonServer;
class vrpn_Connection_IP;

class VRPNServerThread : public QThread
{
	Q_OBJECT
public:
	explicit VRPNServerThread(QObject* parent = 0);
	virtual ~VRPNServerThread();

public slots:
	void setButtonState(unsigned int which, int state);

protected:
	virtual void run();

private:
	bool                m_stopRequested;
	vrpn_Connection_IP* m_connection;
	myButtonServer*     m_buttonServer;

};
#endif // VRPN_SERVER_THREAD_H
