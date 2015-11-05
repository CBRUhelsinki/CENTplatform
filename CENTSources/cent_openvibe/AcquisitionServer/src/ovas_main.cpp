#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include "ovasCAcquisitionServerThread.h"
#include "ovasCCommandLineConfiguration.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

void handleServerThread(CAcquisitionServerThread& thread, IKernelContext& context)
{
	if (!thread.connect()) 
	{
		context.getLogManager() << LogLevel_Fatal << "couldn't connect to driver\n";
		return;
	}
	if (!thread.start())
	{
		context.getLogManager() << LogLevel_Fatal << "couldn't start server\n";
		return;
	}
			
	string input;
	while (input != "c") 
	{
		cin >> input;
	}
}

void startAcquisition(IKernelContext& context, CommandLineConfiguration& config)
{
	CAcquisitionServerThread serverThread(context, config);
	CAcquisitionServerThreadHandle threadHandle(serverThread);
	boost::thread boostThread(threadHandle);
	try
	{
		handleServerThread(serverThread, context);
	}
	catch(...)
	{
		context.getLogManager() << LogLevel_Fatal << "Catched top level exception\n";
	}

	serverThread.stop();
	serverThread.disconnect();
	serverThread.terminate();
	boostThread.join();
}

int main(int argc, char ** argv)
{
//___________________________________________________________________//
//                                                                   //
	CommandLineConfiguration config = CommandLineConfiguration::buildFromArgs(argc, argv);

	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString m_sError;
#if defined OVAS_OS_Windows
	if(!l_oKernelLoader.load("../bin/OpenViBE-kernel-dynamic.dll", &m_sError))
#else
	if(!l_oKernelLoader.load("../lib/libOpenViBE-kernel-dynamic.so", &m_sError))
#endif
	{
			cout<<"[ FAILED ] Error loading kernel ("<<m_sError<<")"<<endl;
			return 0;
	}

	cout<<"[  INF  ] Kernel module loaded, trying to get kernel descriptor"<<endl;
	IKernelDesc* l_pKernelDesc=NULL;
	IKernelContext* l_pKernelContext=NULL;
	l_oKernelLoader.initialize();
	l_oKernelLoader.getKernelDesc(l_pKernelDesc);
	if(!l_pKernelDesc)
	{
		cout<<"[ FAILED ] No kernel descriptor"<<endl;
	}
	else
	{
		cout<<"[  INF  ] Got kernel descriptor, trying to create kernel"<<endl;
		l_pKernelContext=l_pKernelDesc->createKernel("acquisition-server", "../share/openvibe.conf");
		if(!l_pKernelContext)
		{
			cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
		}
		else
		{
			OpenViBEToolkit::initialize(*l_pKernelContext);
			if (config.isTraceEnabled())
			{
				l_pKernelContext->getLogManager().activate(LogLevel_Trace, true);
				l_pKernelContext->getLogManager().activate(LogLevel_Debug, true);
			}

			IConfigurationManager& l_rConfigurationManager=l_pKernelContext->getConfigurationManager();

			l_pKernelContext->getPluginManager().addPluginsFromFiles(l_rConfigurationManager.expand("${Kernel_Plugins}"));

			startAcquisition(*l_pKernelContext, config);

			cout<<"[  INF  ] Application terminated, releasing allocated objects"<<endl;

			OpenViBEToolkit::uninitialize(*l_pKernelContext);

			l_pKernelDesc->releaseKernel(l_pKernelContext);
		}
	}
	l_oKernelLoader.uninitialize();
	l_oKernelLoader.unload();

	return 0;
}
