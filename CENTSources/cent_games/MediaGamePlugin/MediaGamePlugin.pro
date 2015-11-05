PROJECT   = MediaGamePlugin
include($$PWD/GameConfiguration.pri)
include($$GAMES_COMMON/GamePluginConf.pri)

############################################## INCLUDE PATHS
INCLUDEPATH += $$CENT_COMMON_INTERFACES
INCLUDEPATH += $$CENT_COMMON
INCLUDEPATH += $$GAMES_COMMON
INCLUDEPATH += $$SDK_DIR/qtmobility/include

win32 {
	CONFIG(release, debug|release) {
		LIBS += -l$$SDK_DIR/qtmobility/lib/QtMultimediaKit1
	} else {
		LIBS += -l$$SDK_DIR/qtmobility/lib/QtMultimediaKitd1
	}
}

include($$PWD/src/sources.pri)
