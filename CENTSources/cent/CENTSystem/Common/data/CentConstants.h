#ifndef CENT_CONSTANTS_H
#define CENT_CONSTANTS_H

namespace CENT
{
	namespace Stimulation
	{
		const int ON  = 1;
		const int OFF = 0;

		//Common
		const int SCENARIO_CONTROL = 0;

		//Baseline scenario
		const int BASELINE_RECORDING = 1;


		//Other scenario can start from "1" again, "0" is reserver for scenario termination
	}
	
	namespace Settings
	{
		const QString CENT_ORGANIZATION_NAME       = "CENT";
		const QString CENT_APPLICATION_NAME        = "Settings";

		const QString OPENVIBE_INSTALLATION_FOLDER = "OpenVibeInstallDir";
		const QString PARENT_DATA_DIR              = "CentDataDir";
		const QString DATA_REPLICATION_PATH        = "DataReplicationPath";
		const QString CAP_DRIVER_TYPE              = "CapDriverType";
		const QString CAP_DRIVER_CMD               = "CapDriverCmd";
		const QString CAP_DRIVER_MAC_ADDRESS       = "CapDriverMac";
	}

	namespace DirectoryName
	{
		const QString CAP_DRIVER_4CHANNELS_FOLDER_NAME = "/4Channels/";
		const QString CAP_DRIVER_8CHANNELS_FOLDER_NAME = "/8Channels/";
	}

	namespace DirectoryId
	{
		enum Id
		{
			DirectoryCurrentSession = 1,
			DirectoryCurrentGame
		};
	}

	namespace FileNameId
	{
		enum Id
		{
			FileNameBetaCoeff = 1,
			FileNameThetaCoeff
		};
	}
}

#endif // CENT_CONSTANTS_H
