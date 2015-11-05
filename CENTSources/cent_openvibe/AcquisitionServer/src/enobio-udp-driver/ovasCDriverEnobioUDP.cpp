#include "ovasCDriverEnobioUDP.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <system/Memory.h>
#include <system/Time.h>

#include <cmath>

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

#define STARLAB_ENOBIO_FREQUENCY_RATE 250
#define NUMBER_OF_CHANNELS 4

//___________________________________________________________________//
//                                                                   //

CDriverEnobioUDP::CDriverEnobioUDP(IDriverContext& rDriverContext, CommandLineConfiguration& config)
	:IDriver(rDriverContext)
	,m_ui32SampleCountPerSentBlock(0)
	,m_sHostName(config.getEnobioHost().c_str())
	,m_ui32HostPort(config.getEnobioPort())
{
	m_oHeader.setSamplingFrequency(STARLAB_ENOBIO_FREQUENCY_RATE);
	m_oHeader.setChannelCount(NUMBER_OF_CHANNELS);
}

CDriverEnobioUDP::~CDriverEnobioUDP()
{
}

const char* CDriverEnobioUDP::getName(void)
{
	return "Enobio UDP Driver";
}

//___________________________________________________________________//
//                                                                   //


boolean CDriverEnobioUDP::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	if(m_rDriverContext.isConnected()) 
	{ 
		return false; 
	}

	m_ui32DataFrameSize=sizeof(uint16)*m_oHeader.getChannelCount();

	m_pSample=new float32[m_oHeader.getChannelCount()];
	m_pDataFrame=new uint8[m_ui32DataFrameSize];
	
	if(!m_pSample || !m_pDataFrame)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not allocate memory !\n";
		return false;
	}

	if(!this->open())
	{
		return false;
	}

	m_pCallback=&rCallback;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;

	return true;
}


boolean CDriverEnobioUDP::start(void)
{
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}
	return !m_rDriverContext.isStarted();
}

boolean CDriverEnobioUDP::loop(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }

	uint32 i, j;
	for(j=0; j<m_ui32SampleCountPerSentBlock; j++)
	{
		if(!this->read())
		{
			return false;
		}

		for(i=0; i<m_oHeader.getChannelCount(); i++)
		{
			uint8* l_pDataFrame=m_pDataFrame+i*sizeof(uint16);
			uint16 value;
			System::Memory::littleEndianToHost(l_pDataFrame, &value);
			m_pSample[i] = float32(value);
		}

		if(m_rDriverContext.isStarted())
		{
			m_pCallback->setSamples(m_pSample, 1);
		}
	}

	if(m_rDriverContext.isStarted())
	{
		m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());
	}

	return true;
}

boolean CDriverEnobioUDP::stop(void)
{
	if(!m_rDriverContext.isConnected()) 
	{
		return false;
	}
	return m_rDriverContext.isStarted();
}

boolean CDriverEnobioUDP::uninitialize(void)
{
	if(!m_rDriverContext.isConnected()) 
	{ 
		return false; 
	}
	if(m_rDriverContext.isStarted()) 
	{ 
		return false; 
	}

	if(!this->close())
	{
		return false;
	}

	delete [] m_pSample;
	delete [] m_pDataFrame;
	m_pSample=NULL;
	m_pDataFrame=NULL;
	m_pCallback=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //
boolean CDriverEnobioUDP::isConfigurable(void)
{
	return false;
}

boolean CDriverEnobioUDP::configure(void)
{
	return false;
}

boolean CDriverEnobioUDP::open(void)
{
	m_pConnection=Socket::createConnectionClient();
	if(!m_pConnection)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not create client connection\n";
		return false;
	}
	if(!m_pConnection->connect(m_sHostName.toASCIIString(), m_ui32HostPort))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not connect to server [" << m_sHostName << ":" << m_ui32HostPort << "]\n";
		return false;
	}
	return true;
}

boolean CDriverEnobioUDP::close(void)
{
	if(m_pConnection)
	{
		m_pConnection->close();
		m_pConnection->release();
		m_pConnection=NULL;
	}
	return true;
}

boolean CDriverEnobioUDP::read(void)
{
	if(!m_pConnection)
	{
		return false;
	}
	return m_pConnection->receiveBufferBlocking(m_pDataFrame, m_ui32DataFrameSize);
}
