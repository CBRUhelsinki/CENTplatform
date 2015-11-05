TOP_DIR 		= $$PWD/..
CENT_COMMON 	= $$TOP_DIR/Common
APP_SRC_COMMON 	= $$PWD/src/common
SDK_DIR 		= $$TOP_DIR/SDK

DEFINES         += QWT_DLL

## Compile in parallel ##
QMAKE_CXXFLAGS += -MP

## Disable optimization in debug builds
QMAKE_CXXFLAGS_DEBUG += -Od