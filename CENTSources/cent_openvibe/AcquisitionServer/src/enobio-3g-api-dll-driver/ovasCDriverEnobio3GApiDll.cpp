#include "ovasCDriverEnobio3GApiDll.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <system/Memory.h>
#include <system/Time.h>
#include <boost/thread.hpp>

#include "enobio3g/includes/enobio3g.h"
#include "enobio3g/includes/channeldata.h"
#include "enobio3g/includes/StatusData.h"

using namespace OpenViBEAcquisitionServer;

namespace
{
	// Size of the internal buffer
	const OpenViBE::uint32 BUFFER_SCALE_FACTOR = 3;

	//TODO: shouldn't it be pulled from EnobioApi.dll dynamically
	const OpenViBE::uint32 FREQUENCY_RATE = 250u;

	// Supported number of channels
	const OpenViBE::uint32 NUMBER_OF_CHANNELS = 8;

	// Mac address of the device
	const size_t MAC_SIZE = 6;
	const unsigned char MAC[MAC_SIZE] = {0x6a, 0xfb, 0x4b, 0x80, 0x07, 0x00};

	// Name of the driver
	const char * const DRIVER_NAME = "Enobio API 3G DLL Driver";

	// Helper class that transfer data from Enobio to correct method. It does all casting stuff.
	template <class T, class Callback>
	class Consumer : public IDataConsumer
	{
	public:
		Consumer(Callback& callback)
			: m_callback(callback)
		{ }

		void receiveData(const PData& rData)
		{
			// Just cast and call callback
			m_callback.receiveData(*static_cast<const T*>(rData.getData()));
		}

	private:
		Callback& m_callback;
	};

	// Data used to store information about received sample
	typedef std::vector<OpenViBE::float32> SamplesBuffer;
}

class CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate
{
public:
	CDriverEnobio3GApiDllPrivate(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
	~CDriverEnobio3GApiDllPrivate();

	// Method called when enobio data arrived
	void receiveData(const ChannelData& rData);

	// Method called when new status arrived
	void receiveData(const StatusData& rStatus);

	// Parse mac member to format specific for the api
	OpenViBE::boolean parseMac(unsigned char mac[MAC_SIZE]);

	// Open the connection
	OpenViBE::boolean open();

	// Close the connection
	void close();

	// Set number of samples that driver expects
	void setMaxSampleCount(OpenViBE::uint32 ui32SampleCountPerSentBlock);

	// If there is enough samples in buffer, it switches buffers and returns a reference for data
	// that are available for free access until another call of getSamples. If it returns 0, then
	// there is no enough samples in buffer yet
	const SamplesBuffer* getSamples();

	// mac addres for the cap
	std::string m_macAddress;
private:
	// Helper that forwards data to receiveData(const EnobioData& rData)
	Consumer<ChannelData, CDriverEnobio3GApiDllPrivate> m_oDataConsumer;

	// Helper that forwards status to receiveData(const StatusData& rStatus)
	Consumer<StatusData, CDriverEnobio3GApiDllPrivate> m_oStatusConsumer;

	// Enobio driver API
	Enobio3G m_oEnobio3G;

	// Driver context for logging
	OpenViBEAcquisitionServer::IDriverContext& m_rDriverContext;

	// Current number of samples in buffer
	OpenViBE::uint32 m_uiSamplesCount;

	// Buffer for samples
	SamplesBuffer m_aSamplesBuffer;

	// Buffer for reader, we copy samples buffer there
	SamplesBuffer m_aReaderBuffer;

	// Maximum number of samples
	OpenViBE::uint32 m_ui32MaxSamplesCount;

	// Timemark for debug purpose
	OpenViBE::uint32 m_ui32TimeMark;

	// Mutex to protect samples
	boost::mutex m_oSamplesMutex;
};

CDriverEnobio3GApiDll::CDriverEnobio3GApiDll(IDriverContext& rDriverContext, CommandLineConfiguration& config)
	: IDriver(rDriverContext)
	, m_pCallback(NULL)
	, m_pPrivate(new CDriverEnobio3GApiDllPrivate(rDriverContext))
{
	m_oHeader.setSamplingFrequency(FREQUENCY_RATE);
	m_oHeader.setChannelCount(NUMBER_OF_CHANNELS);
	m_pPrivate->m_macAddress = config.getEnobioMacAddress();
}

CDriverEnobio3GApiDll::~CDriverEnobio3GApiDll()
{
	delete m_pPrivate;
	m_pPrivate = NULL;
	m_pCallback = NULL;
}

const char* CDriverEnobio3GApiDll::getName(void)
{
	return DRIVER_NAME;
}

OpenViBE::boolean CDriverEnobio3GApiDll::initialize(const OpenViBE::uint32 ui32SampleCountPerSentBlock,
													 IDriverCallback& rCallback)
{
	if (m_rDriverContext.isConnected())
	{
		return false;
	}

	if (! m_pPrivate->open())
	{
		return false;
	}

	m_pCallback = &rCallback;
	m_pPrivate->setMaxSampleCount(ui32SampleCountPerSentBlock);

	return true;
}


OpenViBE::boolean CDriverEnobio3GApiDll::start(void)
{
	if (! m_rDriverContext.isConnected())
	{
		return false;
	}
	return ! m_rDriverContext.isStarted();
}

OpenViBE::boolean CDriverEnobio3GApiDll::loop(void)
{
	if (! m_rDriverContext.isConnected())
	{
		return false; 
	}

	// Check if there are samples available
	const SamplesBuffer* l_paSamples = m_pPrivate->getSamples();
	if (l_paSamples && m_pCallback)
	{
		// We have samples, let's forward them to callback
		if (m_rDriverContext.isStarted())
		{
/*			OpenViBE::float32 a[4] = { 100, 0, 0, 0 };
			for (int i = 0; i <l_paSamples->size() / EnobioData::NUMBER_OF_CHANNELS; ++i)
			{
				a[1] = i;
				m_pCallback->setSamples(a, 1);
			}*/
			m_pCallback->setSamples(l_paSamples->data(), l_paSamples->size() / NUMBER_OF_CHANNELS);
			m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());
		}
	}

	return true;
}

OpenViBE::boolean CDriverEnobio3GApiDll::stop(void)
{
	if (! m_rDriverContext.isConnected()) 
	{
		return false;
	}
	return m_rDriverContext.isStarted();
}

OpenViBE::boolean CDriverEnobio3GApiDll::uninitialize(void)
{
	if (! m_rDriverContext.isConnected())
	{
		return false; 
	}
	if (m_rDriverContext.isStarted())
	{
		return false; 
	}

	m_pCallback = NULL;
	m_pPrivate->close();
	return true;
}

OpenViBE::boolean CDriverEnobio3GApiDll::isConfigurable(void)
{
	return false;
}

OpenViBE::boolean CDriverEnobio3GApiDll::configure(void)
{
	return false;
}

const OpenViBEAcquisitionServer::IHeader* CDriverEnobio3GApiDll::getHeader(void)
{
	return &m_oHeader;
}

//
//
//
// -----------------------------------------------------------------------------------------------
//
//
//

CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::CDriverEnobio3GApiDllPrivate(OpenViBEAcquisitionServer::IDriverContext& rDriverContext)
	: m_oDataConsumer(*this)
	, m_oStatusConsumer(*this)
	, m_rDriverContext(rDriverContext)
	, m_uiSamplesCount(0)
	, m_ui32MaxSamplesCount(0)
	, m_ui32TimeMark(0)
{
	// Configure channels for driver
	//m_oEnobio.setProperty(Property(Enobio::STR_PROPERTY_ENABLE_CHANNEL_1, EnobioData::NUMBER_OF_CHANNELS > 0));
	//m_oEnobio.setProperty(Property(Enobio::STR_PROPERTY_ENABLE_CHANNEL_2, EnobioData::NUMBER_OF_CHANNELS > 1));
	//m_oEnobio.setProperty(Property(Enobio::STR_PROPERTY_ENABLE_CHANNEL_3, EnobioData::NUMBER_OF_CHANNELS > 2));
	//m_oEnobio.setProperty(Property(Enobio::STR_PROPERTY_ENABLE_CHANNEL_4, EnobioData::NUMBER_OF_CHANNELS > 3));

	m_oEnobio3G.registerConsumer(Enobio3G::ENOBIO_DATA, m_oDataConsumer);
	m_oEnobio3G.registerConsumer(Enobio3G::STATUS,      m_oStatusConsumer);
}

CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::~CDriverEnobio3GApiDllPrivate()
{
	m_oEnobio3G.closeDevice();

	m_oEnobio3G.deregisterConsumer(Enobio3G::ENOBIO_DATA, m_oDataConsumer);
	m_oEnobio3G.deregisterConsumer(Enobio3G::STATUS,      m_oStatusConsumer);
}

OpenViBE::boolean CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::parseMac(unsigned char mac[MAC_SIZE])
{
	std::string strMac(m_macAddress);
	int j = strMac.find(':');
	while(j > 0 && j != std::string::npos)
	{
		strMac.erase(j, 1);
		j = strMac.find(':');
	}
	if(strMac.size() != MAC_SIZE * 2)
	{
		memcpy(mac, MAC, MAC_SIZE);
	}
	else
	{
		char tempMac[MAC_SIZE * 2];
		memcpy(tempMac, strMac. data(), MAC_SIZE * 2);
		char* ptrBegin = NULL;
		int idx = 0;
		for(int i = 0; i < MAC_SIZE; i++)
		{
			idx = 2 * (MAC_SIZE - 1 - i); // 2 chars per value and it is reversed
			ptrBegin = tempMac + idx * sizeof(char);
			mac[i] = static_cast<unsigned char>(strtoul(ptrBegin, NULL, 16) & 0xFF);
			*ptrBegin = 0; // end the string here
		}
	}
	return true;
}

OpenViBE::boolean CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::open()
{
	unsigned char mac[MAC_SIZE];
	if(! parseMac(mac))
	{
		return false;
	}

	if (! m_oEnobio3G.openDevice(mac))
	{
		m_rDriverContext.getLogManager() << OpenViBE::Kernel::LogLevel_Error << "Could not connect to device\n";
		return false;
	}

	m_ui32TimeMark = System::Time::getTime();
	m_oEnobio3G.startStreaming();

	return true;
}

void CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::close()
{
	m_oEnobio3G.stopStreaming();
	m_oEnobio3G.closeDevice();
}

void CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::receiveData(const ChannelData& rData)
{
	ChannelData channelData(rData);
	// Filter strange stuff
	if ((channelData.channelInfo() & 0xFF) != 0xFF) // only 8 channels are supported for now
	{
		return;
	}

	boost::mutex::scoped_lock lock(m_oSamplesMutex);

	// If there is a space in the buffer, add new data
	if (m_uiSamplesCount + 1< m_aSamplesBuffer.size() / NUMBER_OF_CHANNELS)
	{
		OpenViBE::int32 l_i32Value = 0;
		OpenViBE::float32 l_f32Previous = 0.0f;

		OpenViBE::uint32 l_loopRound = m_uiSamplesCount / m_ui32MaxSamplesCount;
		OpenViBE::uint32 l_indexOffset = m_uiSamplesCount - l_loopRound * m_ui32MaxSamplesCount;
		OpenViBE::uint32 l_baseIndex = l_loopRound * m_ui32MaxSamplesCount * NUMBER_OF_CHANNELS;
		// Copy data to buffer
		for (OpenViBE::uint32 i = 0; i < NUMBER_OF_CHANNELS; ++i)
		{
			l_i32Value = channelData.data()[i];

			if (m_uiSamplesCount > 0)
			{
				l_f32Previous = m_aSamplesBuffer[m_uiSamplesCount - 1 + i * m_ui32MaxSamplesCount];
			}
			m_aSamplesBuffer[l_baseIndex + l_indexOffset  + i * m_ui32MaxSamplesCount] = OpenViBE::float32(l_i32Value);
		}
		m_uiSamplesCount += 1;
	}
	else
	{
		m_rDriverContext.getLogManager() << OpenViBE::Kernel::LogLevel_Warning << "Too many data, skipping\n";
	}
}

void CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::receiveData(const StatusData& rData)
{
	//TODO: Different thread issue, fix it
	m_rDriverContext.getLogManager() << OpenViBE::Kernel::LogLevel_Info << "New status arrived " << rData.getCode() << ": " << rData.getString() << "\n";
}

void CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::setMaxSampleCount(OpenViBE::uint32 ui32SampleCountPerSentBlock)
{
	boost::mutex::scoped_lock lock(m_oSamplesMutex);

	m_ui32MaxSamplesCount = ui32SampleCountPerSentBlock;

	// Clear buffer and resize it to new value
	m_uiSamplesCount = 0;
	m_aSamplesBuffer.clear();
	m_aSamplesBuffer.resize(m_ui32MaxSamplesCount * NUMBER_OF_CHANNELS * BUFFER_SCALE_FACTOR);
	m_aReaderBuffer.resize(m_ui32MaxSamplesCount * NUMBER_OF_CHANNELS);
}

const SamplesBuffer* CDriverEnobio3GApiDll::CDriverEnobio3GApiDllPrivate::getSamples()
{
	boost::mutex::scoped_lock lock(m_oSamplesMutex);

	if (m_uiSamplesCount >= m_ui32MaxSamplesCount)
	{
#if 0
		OpenViBE::uint32 ui32Mark = System::Time::getTime();
		m_rDriverContext.getLogManager() << OpenViBE::Kernel::LogLevel_Info << 
			"Got " << m_ui32MaxSamplesCount << " after " << (ui32Mark - m_ui32TimeMark) << "ms\n";
		m_ui32TimeMark = ui32Mark;
#endif
		const OpenViBE::uint32 ui32ItemsToCopy = m_ui32MaxSamplesCount * NUMBER_OF_CHANNELS;
		std::copy(m_aSamplesBuffer.begin(), m_aSamplesBuffer.begin() + ui32ItemsToCopy, m_aReaderBuffer.begin());
		std::copy(m_aSamplesBuffer.begin() + ui32ItemsToCopy, m_aSamplesBuffer.end(), m_aSamplesBuffer.begin());
		m_uiSamplesCount -= m_ui32MaxSamplesCount;
		return &m_aReaderBuffer;
	}

	return NULL;
}
