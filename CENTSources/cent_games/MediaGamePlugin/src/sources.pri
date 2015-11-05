############################################## INTERFACES
HEADERS += $$CENT_COMMON_INTERFACES/ICentGamePlugin.h

############################################# CENT connections HEADERS
HEADERS += $$CENT_COMMON/Connect.h

############################################## ADVANCED MEDIA GAME HEADERS
HEADERS += $$PWD/MediaGamePlugin.h
HEADERS += $$PWD/GameWidget.h
HEADERS += $$PWD/VideoWidgetSurface.h
HEADERS += $$PWD/Settings.h
HEADERS += $$PWD/SettingsWidget.h
HEADERS += $$PWD/AlgorithmManager.h
HEADERS += $$PWD/IAlgorithm.h
HEADERS += $$PWD/ObfuscationAlgorithm.h
HEADERS += $$PWD/ScoreAlgorithm.h
HEADERS += $$PWD/MediaGameDataTypes.h
HEADERS += $$PWD/MediaGameSummary.h
HEADERS += $$PWD/ScoreWidget.h

############################################## ADCANCED MEDIA GAME SOURCES
SOURCES += $$PWD/MediaGamePlugin.cpp
SOURCES += $$PWD/GameWidget.cpp
SOURCES += $$PWD/VideoWidgetSurface.cpp
SOURCES += $$PWD/Settings.cpp
SOURCES += $$PWD/SettingsWidget.cpp
SOURCES += $$PWD/AlgorithmManager.cpp
SOURCES += $$PWD/ObfuscationAlgorithm.cpp
SOURCES += $$PWD/ScoreAlgorithm.cpp
SOURCES += $$PWD/MediaGameDataTypes.cpp
SOURCES += $$PWD/MediaGameSummary.cpp
SOURCES += $$PWD/ScoreWidget.cpp

############################################# CENT connections SOURCES
SOURCES += $$CENT_COMMON/Connect.cpp
