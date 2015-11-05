TEMPLATE 	= app
QT			+= testlib
QT			+= opengl
CONFIG 		+= console

############################################## LIBS
LIBS += -L$$PWD/../vld/lib/Win32
LIBS += -l$$PWD/../vld/lib/Win32/vld

############################################## INCLUDEPATHS
INCLUDEPATH += $(VRPN)
INCLUDEPATH += $$PWD/../vld/include
INCLUDEPATH += $$PWD

############################################## TEST FRAMEWORK
HEADERS += $$PWD/CentUnitTest.h
SOURCES += $$PWD/CentUnitTest.cpp