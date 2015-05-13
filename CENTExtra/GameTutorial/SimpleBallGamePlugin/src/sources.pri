############################################## INTERFACES
HEADERS += $$CENT_COMMON_INTERFACES/ICentGamePlugin.h

############################################# CENT connections HEADERS
HEADERS += $$CENT_COMMON/Connect.h

############################################# GAME COMMON HEADERS
HEADERS += $$GAMES_COMMON/GLWidget.h

############################################## ADVANCED BALL GAME HEADERS
HEADERS += $$PWD/SimpleBallGamePlugin.h
HEADERS += $$PWD/SimpleBallGameLogic.h
HEADERS += $$PWD/SimpleBallGameSummary.h
HEADERS += $$PWD/SimpleBallGameWidget.h

############################################## ADCANCED BALL GAME SOURCES
SOURCES += $$PWD/SimpleBallGamePlugin.cpp
SOURCES += $$PWD/SimpleBallGameLogic.cpp
SOURCES += $$PWD/SimpleBallGameSummary.cpp
SOURCES += $$PWD/SimpleBallGameWidget.cpp

############################################# CENT connections SOURCES
SOURCES += $$CENT_COMMON/Connect.cpp

############################################# GAME COMMON SOURCES
SOURCES += $$GAMES_COMMON/GLWidget.cpp