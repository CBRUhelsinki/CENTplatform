include($$PWD/Configuration.pri)

TEMPLATE    =  app
PROJECT     =  MappingTool
CONFIG      *= debug_and_releases
CONFIG      += console
DESTDIR     =  $$TOP_DIR/output
OBJECTS_DIR =  $$PWD/output/obj
MOC_DIR     =  $$PWD/output/moc

QT          -= gui

CONFIG(release, debug|release){
	TARGET = $${PROJECT}
} else {
	TARGET = $${PROJECT}d
}

# Build the 32-bit version
QMAKE_LFLAGS   += /MACHINE:X86
# Enable the compiler to detect more problems
DEFINES        += STRICT

############################################## SOURCES.PRI
include($$PWD/src/sources.pri)


############################################## MAIN
SOURCES += $$PWD/src/main.cpp

############################################## RESOURCES
RC_FILE = $$PWD/resources/resources.rc