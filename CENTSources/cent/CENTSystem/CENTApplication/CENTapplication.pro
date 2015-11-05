include($$PWD/Configuration.pri)

TEMPLATE    =  app
PROJECT     =  CENTapplication
CONFIG      *= debug_and_releases
DESTDIR     =  $$TOP_DIR/output
OBJECTS_DIR =  $$PWD/output/obj
MOC_DIR     =  $$PWD/output/moc

QT          += svg
QT          += webkit

CONFIG(release, debug|release){
	TARGET = $${PROJECT}
} else {
	TARGET = $${PROJECT}d
}

win32 {
	CONFIG(release, debug|release) {
		LIBS += -l$(VRPN)/pc_win32/Release/vrpn
		LIBS += -l$$SDK_DIR/qwt/lib/qwt
	} else {
		LIBS += -l$(VRPN)/pc_win32/Debug/vrpn
		LIBS += -l$$SDK_DIR/qwt/lib/qwtd
	}

	# Build the 32-bit version
	QMAKE_LFLAGS   += /MACHINE:X86
	# Enable the compiler to detect more problems
	DEFINES        += STRICT
}

include($$PWD/src/sources.pri)

############################################## INCLUDE PATHS
INCLUDEPATH  += $(VRPN)

############################################## LIBRARIES
LIBS += -L$(VRPN)/pc_win32/Debug
LIBS += -lvrpn

############################################## MAIN
SOURCES += $$PWD/src/main.cpp

############################################## RESOURCES
RC_FILE = $$PWD/resources/resources.rc