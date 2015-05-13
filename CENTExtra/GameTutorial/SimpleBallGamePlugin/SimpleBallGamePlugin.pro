PROJECT   = SimpleBallGamePlugin
include($$PWD/GameConfiguration.pri)
include($$GAMES_COMMON/GamePluginConf.pri)

QT += opengl

############################################## INCLUDE PATHS
INCLUDEPATH += $$CENT_COMMON_INTERFACES
INCLUDEPATH += $$CENT_COMMON
INCLUDEPATH += $$GAMES_COMMON
INCLUDEPATH += $$PWD/src

include($$PWD/src/sources.pri)
