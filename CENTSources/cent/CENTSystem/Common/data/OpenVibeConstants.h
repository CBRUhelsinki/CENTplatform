#ifndef OPENVIBE_CONSTANTS_H
#define OPENVIBE_CONSTANTS_H

namespace OPENVIBE
{
	const QString BIN_DIRECTORY = "bin";
	const QString ALTERNATE_BIN_DIRECTORY = "dist/bin";

	const QString ACQ_SERVER_EXE_NAME = "OpenViBEAcquisitionServerDynamic.exe";
	const QString OPENVIBE_EXE_NAME = "OpenViBEDesignerDynamic.exe";

	namespace RUN_PARAMS
	{
		// OpenVibe designer
		const QString NO_GUI = "--no-gui";
		const QString NO_MANAGEMENT = "--no-session-management";
		const QString PLAY_SCENARIO = "--play";

	}

	namespace DEPS
	{
		const QString DIRECTORY_PATH = "dependencies";

		const QString SUBPATH_LIBEXPAT       = "/expat/bin;";
		const QString SUBPATH_GTK            = "/gtk/bin;";
		const QString SUBPATH_OGRE_DEBUG     = "/ogre/bin/debug;";
		const QString SUBPATH_OGRE_RELEASE   = "/ogre/bin/release;";
		const QString SUBPATH_ITPP           = "/itpp/bin;";
		const QString SUBPATH_LUA            = "/lua/lib;";
		const QString SUBPATH_OPENAL         = "/openal/libs/Win32;";
		const QString SUBPATH_FREEALUT       = "/freealut/lib;";
		const QString SUBPATH_VORBIS_DEBUG   = "/libvorbis/win32/bin/debug;";
		const QString SUBPATH_VORBIS_RELEASE = "/libvorbis/win32/bin/release;";
		const QString SUBPATH_OGG_DEBUG      = "/libogg/win32/bin/debug;";
		const QString SUBPATH_OGG_RELEASE    = "/libogg/win32/bin/release;";
	}

	enum DesignerScenarioType
	{
		EEGWithNoise = 0,
		BaseLine,
		BaseLinePostProcess,
		Game
	};
}

#endif // OPENVIBE_CONSTANTS_H
