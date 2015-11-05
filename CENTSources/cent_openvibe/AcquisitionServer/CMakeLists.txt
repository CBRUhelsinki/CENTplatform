PROJECT(OpenViBE-acquisition-server)

SET(PROJECT_VERSION_MAJOR 0)
SET(PROJECT_VERSION_MINOR 12)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h)
ADD_EXECUTABLE(${PROJECT_NAME}-dynamic ${source_files})

INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBEToolkit")
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleSocket")
INCLUDE("FindOpenViBEModuleSystem")
INCLUDE("FindOpenViBEPluginsGlobalDefines")
INCLUDE("FindThirdPartyBoost_Thread")
INCLUDE("FindThirdPartyGTK")
INCLUDE("FindThirdPartyGUSBampCAPI")
INCLUDE("FindThirdPartyGMobiLabPlusAPI")
INCLUDE("FindThirdPartyUSBFirstAmpAPI")
INCLUDE("FindThirdPartyThinkGearAPI")
INCLUDE("FindThirdPartyEmotivAPI")
INCLUDE("FindThirdPartyOpenAL")

INCLUDE_DIRECTORIES(.)

IF(WIN32)
	# Enobio 4 channel cap api
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${CMAKE_CURRENT_SOURCE_DIR}/enobio/libs/EnobioAPI.lib)

	# Enobio 8/20 channel cap api
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${CMAKE_CURRENT_SOURCE_DIR}/enobio3g/libs/Enobio3GAPI.lib)
	
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic delayimp.lib)
	SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES LINK_FLAGS "/DELAYLOAD:Enobio3GAPI.dll /DELAYLOAD:EnobioAPI.dll")
	
ENDIF(WIN32)
# ---------------------------------
# Target macros
# Defines target operating system
# Defines target architecture
# Defines target compilator
# ---------------------------------
IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-DTARGET_OS_Windows)
	ADD_DEFINITIONS(-DTARGET_ARCHITECTURE_i386)
	ADD_DEFINITIONS(-DTARGET_COMPILATOR_VisualStudio)
ENDIF(WIN32)
IF(UNIX)
	# ADD_DEFINITIONS(-fvisibility=hidden) # This flag should be present... man gcc
	ADD_DEFINITIONS(-g)
	ADD_DEFINITIONS(-fnon-call-exceptions)
	ADD_DEFINITIONS(-DTARGET_OS_Linux)
	ADD_DEFINITIONS(-DTARGET_ARCHITECTURE_i386)
	ADD_DEFINITIONS(-DTARGET_COMPILATOR_GCC)
ENDIF(UNIX)

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}-dynamic
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

INSTALL(DIRECTORY bin/   DESTINATION bin     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY doc/   DESTINATION doc     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY etc/   DESTINATION etc     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY lib/   DESTINATION lib     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY share/ DESTINATION share   PATTERN ".svn" EXCLUDE)

#INSTALL(DIRECTORY src/   DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
