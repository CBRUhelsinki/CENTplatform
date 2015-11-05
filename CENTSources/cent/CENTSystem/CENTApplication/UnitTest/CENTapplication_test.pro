include($$PWD/../Configuration.pri)
include($$SDK_DIR/UnitTest/UnitTest.pro)

DESTDIR      = $$PWD/../output/UnitTest
PROJECT      = CENTapplication_test
CONFIG      *= debug_and_releases
OBJECTS_DIR  = $$PWD/../output/UnitTest/obj
MOC_DIR      = $$PWD/../output/UnitTest/moc

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

include($$SDK_DIR/UnitTest/RunUnitTests.pro)

include($$PWD/../src/sources.pri)
include($$PWD/tests/sources.pri)

# use QWT dll
DEFINES        += QWT_DLL

updateqm.commands = lrelease myapp_no.ts
updateqm.target = updateqm
QMAKE_EXTRA_TARGETS += updateqm
