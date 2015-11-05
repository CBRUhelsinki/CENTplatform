#include <QApplication>
#include "CentUnitTest.h"

#include "test_CentStateManager.h"
#include "test_VRPNClientThread.h"
#include "test_CentController.h"
#include "test_CentDataCenter.h"
#include "test_Cipher.h"
#include "test_DynamicDataModel.h"
#include "test_PowerSpectrumData.h"
#include "test_PowerSpectrumFileReader.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	CentUnitTest centUnitTest;

	centUnitTest.addTest(new test_PowerSpectrumData());
	centUnitTest.addTest(new test_PowerSpectrumFileReader());
	centUnitTest.addTest(new test_Cipher());
	centUnitTest.addTest(new test_CentStateManager());
	centUnitTest.addTest(new test_VRPNClientThread());
	centUnitTest.addTest(new test_CentController());
	centUnitTest.addTest(new test_CentDataCenter());
	centUnitTest.addTest(new test_DynamicDataModel());

	return centUnitTest.run(argc, argv);
}
