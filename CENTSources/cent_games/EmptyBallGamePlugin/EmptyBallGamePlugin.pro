PROJECT   = EmptyBallGamePlugin
OUTPUT_DIR = $$PWD/../../output/
include($$PWD/../Common/GamePluginConf.pri)

QT += opengl

############################################## INCLUDE PATHS
INCLUDEPATH += $$PWD/../../common/interfaces
INCLUDEPATH += $$PWD/../../common
INCLUDEPATH += $$PWD/../Common
INCLUDEPATH += $$PWD/src

include($$PWD/src/sources.pri)

############################################## RESOURCES
RC_FILE = $$PWD/resources/resources.rc