#ifndef __OpenViBE_AcquisitionServer_CAcquisitionServerThread_H__
#define __OpenViBE_AcquisitionServer_CAcquisitionServerThread_H__

//include defines that are mostly used for local (debug) builds
#include "ovasCAcquisitionServer.h"

// Drivers
#include "generic-oscilator/ovasCDriverGenericOscilator.h"
#include "enobio-api-dll-driver/ovasCDriverEnobioApiDll.h"
#include "enobio-udp-driver/ovasCDriverEnobioUDP.h"
#include "enobio-3g-api-dll-driver/ovasCDriverEnobio3GApiDll.h"

#include "ovasCCommandLineConfiguration.h"

#include <system/Time.h>
#include <string>

#ifdef TARGET_OS_Windows
 #include <windows.h>
#endif

#define DEFAULT_SAMPLES_PER_CHANNEL 32

namespace OpenViBEAcquisitionServer
{
	class CAcquisitionServerThread
	{
	public:

		enum
		{
			Status_Idle,
			Status_Connected,
			Status_Started,
			Status_Finished
		};

		CAcquisitionServerThread(const OpenViBE::Kernel::IKernelContext& rKernelContext, CommandLineConfiguration& config)
			:m_rKernelContext(rKernelContext)
			,m_ui32Status(Status_Idle)
			,m_rAcquisitionServer(rKernelContext)
			,samplesPerChannel(DEFAULT_SAMPLES_PER_CHANNEL)
			,m_driver(NULL)
			,configuration(config)
		{
		}

		void main(void)
		{
			OpenViBE::float64 l_f64LastDrift=-1;
			OpenViBE::boolean l_bFinished=false;
			while(!l_bFinished)
			{
				OpenViBE::boolean l_bShouldSleep=false;
				OpenViBE::boolean l_bShouldDisconnect=false;
				OpenViBE::float64 l_f64Drift;

				{
					boost::mutex::scoped_lock m_oProtectionLock(m_rAcquisitionServer.m_oProtectionMutex);
					boost::mutex::scoped_lock m_oExecutionLock(m_rAcquisitionServer.m_oExecutionMutex);
					m_oProtectionLock.unlock();

					l_f64Drift=(m_ui32Status==Status_Started?m_rAcquisitionServer.getDrift():0);

					switch(m_ui32Status)
					{
						case Status_Idle:
							l_bShouldSleep=true;
							break;

						case Status_Connected:
						case Status_Started:
							{
								if(!m_rAcquisitionServer.loop())
								{
									l_bShouldDisconnect=true;
								}
								else
								{
									for(unsigned int i=0; i<m_vImpedance.size(); i++)
									{
										m_vImpedance[i]=m_rAcquisitionServer.getImpedance(i);
									}
								}
							}
							break;

						case Status_Finished:
							l_bFinished=true;
							break;

						default:
							break;
					}
				}

				if(!l_bFinished)
				{
					if(m_vImpedance!=m_vImpedanceLast)
					{
						m_vImpedanceLast=m_vImpedance;
					}

					if(l_f64Drift!=l_f64LastDrift)
					{
						l_f64LastDrift=l_f64Drift;
					}

					if(l_bShouldSleep)
					{
						System::Time::sleep(100);
					}
				}
			}

			m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread:: thread finished\n";
		}

		OpenViBE::boolean connect(void)
		{
			boost::mutex::scoped_lock m_oProtectionLock(m_rAcquisitionServer.m_oProtectionMutex);
			boost::mutex::scoped_lock m_oExecutionLock(m_rAcquisitionServer.m_oExecutionMutex);
			m_oProtectionLock.unlock();

			m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::connect()\n";

#ifdef TARGET_OS_Windows
			// If API based driver is enable, let's use it, otherwise use old implementation
			if (configuration.isEnobioApiDriverEnabled())
			{
				HMODULE hMod = LoadLibrary("EnobioApi.dll");
				if(hMod)
				{
					m_driver = new CDriverEnobioApiDll(m_rAcquisitionServer.getDriverContext(), configuration);
				}
			}
			else if (configuration.isEnobio3GApiDriverEnabled())
			{
				HMODULE hMod = LoadLibrary("Enobio3GApi.dll");
				if(hMod)
				{
					m_driver = new CDriverEnobio3GApiDll(m_rAcquisitionServer.getDriverContext(), configuration);
				}
			}
			else
#endif
			if(configuration.isOscilatorEnabled())
			{
				CDriverGenericOscillator::Channels ch;
				switch(configuration.getChannelCount())
				{
				case 8:
					ch = CDriverGenericOscillator::channels_8;
					break;
				case 20:
					ch = CDriverGenericOscillator::channels_20;
					break;
				case 4:
				default:
					ch = CDriverGenericOscillator::channels_4;
					break;
				}
				m_driver = new CDriverGenericOscillator(m_rAcquisitionServer.getDriverContext(), ch);
			}
			//fallback
			if(! m_driver)
			{
				m_driver = new CDriverEnobioUDP(m_rAcquisitionServer.getDriverContext(), configuration);
			}

			if(!m_rAcquisitionServer.connect(*m_driver, header, samplesPerChannel, configuration.getListenPort()))
			{
				return false;
			}
			else
			{
				m_ui32Status=Status_Connected;
			}

			m_vImpedance.resize(header.getChannelCount(), OVAS_Impedance_NotAvailable);
			m_vImpedanceLast.resize(header.getChannelCount(), OVAS_Impedance_NotAvailable);
			return true;
		}

		OpenViBE::boolean start(void)
		{
			boost::mutex::scoped_lock m_oProtectionLock(m_rAcquisitionServer.m_oProtectionMutex);
			boost::mutex::scoped_lock m_oExecutionLock(m_rAcquisitionServer.m_oExecutionMutex);
			m_oProtectionLock.unlock();

			m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::start()\n";
			if(!m_rAcquisitionServer.start())
			{
				m_rAcquisitionServer.stop();
				return false;
			}
			else
			{
				m_ui32Status=Status_Started;
			}
			return true;
		}

		OpenViBE::boolean stop(void)
		{
			boost::mutex::scoped_lock m_oProtectionLock(m_rAcquisitionServer.m_oProtectionMutex);
			boost::mutex::scoped_lock m_oExecutionLock(m_rAcquisitionServer.m_oExecutionMutex);
			m_oProtectionLock.unlock();

			m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::stop()\n";
			m_rAcquisitionServer.stop();
			m_ui32Status=Status_Connected;
			return true;
		}

		OpenViBE::boolean disconnect(void)
		{
			boost::mutex::scoped_lock m_oProtectionLock(m_rAcquisitionServer.m_oProtectionMutex);
			boost::mutex::scoped_lock m_oExecutionLock(m_rAcquisitionServer.m_oExecutionMutex);
			m_oProtectionLock.unlock();

			m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::disconnect()\n";

			if(m_ui32Status==Status_Started)
			{
				m_rAcquisitionServer.stop();
			}

			m_rAcquisitionServer.disconnect();

			delete m_driver;
			m_driver = NULL;

			m_vImpedance.clear();
			m_vImpedanceLast.clear();

			m_ui32Status=Status_Idle;
			return true;
		}

		OpenViBE::boolean terminate(void)
		{
			boost::mutex::scoped_lock m_oProtectionLock(m_rAcquisitionServer.m_oProtectionMutex);
			boost::mutex::scoped_lock m_oExecutionLock(m_rAcquisitionServer.m_oExecutionMutex);
			m_oProtectionLock.unlock();

			m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::terminate()\n";

			switch(m_ui32Status)
			{
				case Status_Started:
					m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::stop()\n";
					m_rAcquisitionServer.stop();
				case Status_Connected:
					m_rKernelContext.getLogManager() << OpenViBE::Kernel::LogLevel_Trace << "CAcquisitionServerThread::disconnect()\n";
					m_rAcquisitionServer.disconnect();
			}

			m_ui32Status=Status_Finished;
			return true;
		}

	protected:
		const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
		OpenViBEAcquisitionServer::CAcquisitionServer m_rAcquisitionServer;
		OpenViBE::uint32 m_ui32Status;
		std::vector < OpenViBE::float64 > m_vImpedanceLast;
		std::vector < OpenViBE::float64 > m_vImpedance;
		OpenViBE::uint32 samplesPerChannel;
		OpenViBEAcquisitionServer::IDriver* m_driver;
		CHeader header;
		CommandLineConfiguration configuration;
	};

	class CAcquisitionServerThreadHandle
	{
	public:

		CAcquisitionServerThreadHandle(OpenViBEAcquisitionServer::CAcquisitionServerThread& rAcquisitionServerThread)
			:m_rAcquisitionServerThread(rAcquisitionServerThread)
		{
		}

		void operator() (void)
		{
			m_rAcquisitionServerThread.main();
		}

	protected:

		OpenViBEAcquisitionServer::CAcquisitionServerThread& m_rAcquisitionServerThread;
	};
}

#endif // __OpenViBE_AcquisitionServer_CAcquisitionServerThread_H__
