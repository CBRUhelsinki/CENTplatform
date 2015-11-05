#ifndef __OpenViBE_AcquisitionServer_CDriverEnobioUDP_H__
#define __OpenViBE_AcquisitionServer_CDriverEnobioUDP_H__

#include <socket/IConnectionClient.h>
#include "../ovasIDriver.h"
#include "../ovasCHeader.h"
#include "../ovasCCommandLineConfiguration.h"


namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CDriverEnobioUDP
	 * \author Piotr Adamski <piotr.adamski@blstream.com> (BLStream)
	 * \date Tue Dec  6 18:38:28 2011
	 * \erief The CDriverEnobioUDP allows the acquisition server to acquire data from
	 * Starlab Enobio device
	 *
	 * \sa CConfigurationCentSystemDriver
	 */
	class CDriverEnobioUDP : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverEnobioUDP(OpenViBEAcquisitionServer::IDriverContext& rDriverContext, 
			OpenViBEAcquisitionServer::CommandLineConfiguration& config);
		virtual ~CDriverEnobioUDP();
		virtual const char* getName(void);

		virtual OpenViBE::boolean initialize(
			const OpenViBE::uint32 ui32SampleCountPerSentBlock,
			OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual OpenViBE::boolean uninitialize(void);

		virtual OpenViBE::boolean start(void);
		virtual OpenViBE::boolean stop(void);
		virtual OpenViBE::boolean loop(void);

		virtual OpenViBE::boolean isConfigurable(void);
		virtual OpenViBE::boolean configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void) { return &m_oHeader; }

	private:

		virtual OpenViBE::boolean open(void);
		virtual OpenViBE::boolean close(void);
		virtual OpenViBE::boolean read(void);
		
		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;
		OpenViBE::float32* m_pSample;
		Socket::IConnectionClient* m_pConnection;
		OpenViBE::CString m_sHostName;
		OpenViBE::uint32 m_ui32HostPort;
		OpenViBE::uint32 m_ui32DataFrameSize;
		OpenViBE::uint8* m_pDataFrame;
		OpenViBEAcquisitionServer::CHeader m_oHeader;
		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
	};
};

#endif // __OpenViBE_AcquisitionServer_CDriverEnobioUDP_H__
