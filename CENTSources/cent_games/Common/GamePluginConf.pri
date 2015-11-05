## $${PROJECT} must be defined before including this configuration

TEMPLATE	= lib
CONFIG 		+= qt plugin

DESTDIR 	= $$OUTPUT_DIR/plugins
OBJECTS_DIR = $$OUTPUT_DIR/plugins/$${PROJECT}/obj
MOC_DIR 	= $$OUTPUT_DIR/plugins/$${PROJECT}/moc

CONFIG(debug, debug|release){
	TARGET = $${PROJECT}d
} else {
	TARGET = $${PROJECT}
}

## Compile in parallel ##
QMAKE_CXXFLAGS += -MP

## Disable optimization in debug builds
QMAKE_CXXFLAGS_DEBUG += -Od