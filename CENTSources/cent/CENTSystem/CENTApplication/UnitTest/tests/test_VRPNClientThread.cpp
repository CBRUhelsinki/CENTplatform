#include "test_VRPNClientThread.h"

#include <QTest>

#include "VRPNClientThread.h"

void test_VRPNClientThread::testMemoryManagement()
{
	VRPNClientThread vrpnClientThread;
	QObject obj;
	QCOMPARE(vrpnClientThread.registerAnalogClient("test_AnalogName", &obj, SLOT(deleteLater())), CENT::Success);
	QCOMPARE(vrpnClientThread.registerDigitalClient("test_digitalName", &obj, SLOT(deleteLater())), CENT::Success);

	VRPNClientThread* vrpnClientThread_heap = new VRPNClientThread();
	QCOMPARE(vrpnClientThread_heap->registerAnalogClient("test_AnalogName", &obj, SLOT(deleteLater())), CENT::Success);
	QCOMPARE(vrpnClientThread_heap->registerDigitalClient("test_digitalName", &obj, SLOT(deleteLater())), CENT::Success);

	delete vrpnClientThread_heap;
}

