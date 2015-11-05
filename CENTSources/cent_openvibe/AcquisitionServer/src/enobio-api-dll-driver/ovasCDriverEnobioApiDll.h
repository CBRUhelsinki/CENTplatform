#ifndef _OpenViBE_AcquisitionServer_CDriverEnobioApiDll_H__
#define _OpenViBE_AcquisitionServer_CDriverEnobioApiDll_H__

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"
#include "../ovasCCommandLineConfiguration.h"

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CDriverEnobioApiDll
	 * \author Pawel Koczan <pawel.koczan@blstream.com> (BLStream)
	 * \date 2012.02.08
	 * \brief The CDriverEnobioApiDll allows the acquisition server to acquire data from
	 * Starlab Enobio device using API
	 *
	 * \sa CConfigurationCentSystemDriver
	 */
	class CDriverEnobioApiDll : public OpenViBEAcquisitionServer::IDriver
	{
	public:
		CDriverEnobioApiDll(OpenViBEAcquisitionServer::IDriverContext& rDriverContext, 
		                    OpenViBEAcquisitionServer::CommandLineConfiguration& config);

		virtual ~CDriverEnobioApiDll();
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
		class CDriverEnobioApiDllPrivate;
		CDriverEnobioApiDllPrivate* m_pPrivate;
	};
};

#endif // __OpenViBE_AcquisitionServer_CDriverEnobioApiDll_H__
