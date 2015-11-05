#ifndef CENT_DATA_TYPES_H
#define CENT_DATA_TYPES_H

#include <QMetaType>

// Structures for data from OpenViBE game scenario
namespace CENT
{
	enum ErrorCode
	{
		Success = 0,
		WrongArgumentError,
		NotSelectedCapDrivers,
		UnknownError
	};
};

namespace CentData
{
	// Max number of channels
	const int CHANNEL_MAX = 128;

	//
	enum GameControlStream
	{
		Classification = 0,
		EpochNotifier
	};

	//
	enum EEGPowerStream
	{
		POWER_THETA_LEVEL = 0,
		POWER_THETA_RANGE,
		POWER_BETA_LEVEL,
		POWER_BETA_RANGE
	};

	//
	enum CapDrivers
	{
		CapDriver4thChannels = 0,
		CapDriver8thChannels,
		UnknowCapDriver
	};

	// Cap drivers structure
	struct CapDriverData
	{
		CapDrivers type;
		QString    name;
		QString    commandToRun;
	};

	// Timestamp structure
	struct time
	{
		// Seconds
		long tv_sec;
		// Miliseconds
		long tv_usec;
	};

	// Digital data structure 
	// Stimulations sent to CENT from OpenViBE scenario
	struct DigitalData
	{
		// Timestamp of button press/release
		struct time msg_time;
		// Which button (numbered from zero)
		int button;
		// Button state (0 = off, 1 = on) 
		int state;
	};

	// Analog data structure
	// Data of any stream matrix sent to CENT from OpenViBE scenario
	struct AnalogData
	{
		// Timestamp of analog data
		struct time msg_time;
		// How many channels
		int num_channel;
		// Analog channel values
		double channel[CHANNEL_MAX];
	};
};

Q_DECLARE_METATYPE(CentData::AnalogData)
Q_DECLARE_METATYPE(CentData::DigitalData)

#endif // CENT_DATA_TYPES_H
