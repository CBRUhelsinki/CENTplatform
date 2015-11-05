############################################## INCLUDEPATHS
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/data
INCLUDEPATH += $$PWD/interfaces
INCLUDEPATH += $$PWD/ui
INCLUDEPATH += $$PWD/logic
INCLUDEPATH += $$PWD/logic/controllers
INCLUDEPATH += $$PWD/vrpn
INCLUDEPATH += $$PWD/ov
INCLUDEPATH += $$PWD/helpers

############################################## INTERFACE HEADERS
HEADERS += $$PWD/interfaces/ICentGamePlugin.h
HEADERS += $$PWD/interfaces/ICentPage.h
HEADERS += $$PWD/interfaces/ICentUserDataManager.h
HEADERS += $$PWD/interfaces/IResultReceiver.h

############################################## COMMON HEADERS
HEADERS += $$PWD/CentDataTypes.h
HEADERS += $$PWD/Connect.h

############################################## Data HEADERS
HEADERS += $$PWD/data/CentActions.h
HEADERS += $$PWD/data/OpenVibeConstants.h
HEADERS += $$PWD/data/CentConstants.h

############################################## OPENVIBE HEADERS
HEADERS += $$PWD/ov/OVAppsRunner.h

############################################## VRPN HEADERS
HEADERS += $$PWD/vrpn/VRPNAnalogClient.h
HEADERS += $$PWD/vrpn/VRPNClientThread.h
HEADERS += $$PWD/vrpn/VRPNDigitalClient.h
HEADERS += $$PWD/vrpn/VRPNServerThread.h

############################################## LOGIC HEADERS
HEADERS += $$PWD/logic/controllers/CentMainWindowControllerBase.h
HEADERS += $$PWD/logic/controllers/BaseCentPageController.h
HEADERS += $$PWD/logic/LogicUiFusion.h
HEADERS += $$PWD/logic/DynamicDataModel.h
HEADERS += $$PWD/logic/CentActionHandler.h
HEADERS += $$PWD/logic/CentErrors.h
HEADERS += $$PWD/logic/Cipher.h

############################################## UI HEADERS
HEADERS += $$PWD/ui/CentMainWindow.h

############################################## HELPER HEADERS
HEADERS += $$PWD/helpers/CentFileHelpers.h

############################################## COMMON SOURCES
SOURCES += $$PWD/Connect.cpp

############################################## OPENVIBE SOURCES
SOURCES += $$PWD/ov/OVAppsRunner.cpp

############################################## VRPN SOURCES
SOURCES += $$PWD/vrpn/VRPNAnalogClient.cpp
SOURCES += $$PWD/vrpn/VRPNClientThread.cpp
SOURCES += $$PWD/vrpn/VRPNDigitalClient.cpp
SOURCES += $$PWD/vrpn/VRPNServerThread.cpp

############################################## LOGIC SOURCES
SOURCES += $$PWD/logic/controllers/CentMainWindowControllerBase.cpp
SOURCES += $$PWD/logic/controllers/BaseCentPageController.cpp
SOURCES += $$PWD/logic/DynamicDataModel.cpp
SOURCES += $$PWD/logic/CentActionHandler.cpp
SOURCES += $$PWD/logic/Cipher.cpp

############################################## UI SOURCES
SOURCES += $$PWD/ui/CentMainWindow.cpp

############################################## HELPER SOURCES
SOURCES += $$PWD/helpers/CentFileHelpers.cpp