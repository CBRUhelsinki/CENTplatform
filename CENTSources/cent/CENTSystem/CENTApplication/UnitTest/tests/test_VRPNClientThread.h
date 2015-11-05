#ifndef TEST_VRPN_CLIENT_THREAD_H
#define TEST_VRPN_CLIENT_THREAD_H

#include <QObject>


class test_VRPNClientThread: public QObject
{
	Q_OBJECT
public:
	explicit test_VRPNClientThread(QObject* parent = 0)
		: QObject(parent)
	{
	}

private slots:
	void testMemoryManagement();
//	void testStartStop(); plans for future
};

#endif // TEST_VRPN_CLIENT_THREAD_H
