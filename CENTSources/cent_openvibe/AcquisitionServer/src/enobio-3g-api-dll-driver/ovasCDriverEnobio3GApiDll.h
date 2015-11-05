#ifndef _OpenViBE_AcquisitionServer_CDriverEnobio3GApiDll_H__
#define _OpenViBE_AcquisitionServer_CDriverEnobio3GApiDll_H__

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"
#include "../ovasCCommandLineConfiguration.h"

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CDriverEnobio3GApiDll
	 * \author Robert Rabenel <robert.rabenel@blstream.com> (BLStream)
	 * \date 2012.07.18
	 * \brief The CDriverEnobioApiDll allows the acquisition server to acquire data from
	 * Starlab Enobio device using API
	 *
	 * \sa CConfigurationCentSystemDriver
	 */
	class CDriverEnobio3GApiDll : public OpenViBEAcquisitionServer::IDriver
	{
	public:
		CDriverEnobio3GApiDll(OpenViBEAcquisitionServer::IDriverContext& rDriverContext, 
		                    OpenViBEAcquisitionServer::CommandLineConfiguration& config);

		virtual ~CDriverEnobio3GApiDll();
		virtual const char* getName(void);

		virtual OpenViBE::boolean initialize(const OpenViBE::uint32 ui32SampleCountPerSentBlock,
		                                     OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual OpenViBE::boolean uninitialize(void);

		virtual OpenViBE::boolean start(void);
		virtual OpenViBE::boolean stop(void);
		virtual OpenViBE::boolean loop(void);

		virtual OpenViBE::boolean isConfigurable(void);
		virtual OpenViBE::boolean configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void);

	private:
		OpenViBEAcquisitionServer::CHeader m_oHeader;
		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;

		//Pimpl to hide implementation
		class CDriverEnobio3GApiDllPrivate;
		CDriverEnobio3GApiDllPrivate* m_pPrivate;
	};
};

#endif // _OpenViBE_AcquisitionServer_CDriverEnobio3GApiDll_H__
