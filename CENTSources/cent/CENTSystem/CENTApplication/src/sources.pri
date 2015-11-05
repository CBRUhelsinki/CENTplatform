include($$PWD/../../Common/sources.pri)

RESOURCES = $$PWD/../resources/CENTResources.qrc

############################################## INCLUDEPATHS
INCLUDEPATH += $$CENT_COMMON
INCLUDEPATH += $(VRPN)
INCLUDEPATH += $$PWD/common
INCLUDEPATH += $$PWD/dataCenter
INCLUDEPATH += $$PWD/ui
INCLUDEPATH += $$PWD/ui/pages
INCLUDEPATH += $$PWD/logic
INCLUDEPATH += $$PWD/logic/controllers
INCLUDEPATH += $$PWD/vrpn
INCLUDEPATH += $$PWD/widgets
INCLUDEPATH += $$PWD/helpers
INCLUDEPATH += $$SDK_DIR/qwt/include

############################################## ERROR HANDLING HEADERS
HEADERS += $$PWD/common/ErrorHandler.h

############################################## DEBUG REDIRECT
HEADERS += $$PWD/common/QDebugRedirect.h

############################################## DATA HEADERS
HEADERS += $$PWD/common/StateEnums.h
HEADERS += $$PWD/common/DataModelCommon.h
HEADERS += $$PWD/common/MainWindowCommon.h

############################################## LOGIC HEADERS
HEADERS += $$PWD/logic/CentController.h
HEADERS += $$PWD/logic/CentGameManager.h
HEADERS += $$PWD/logic/CentGamePluginManager.h
HEADERS += $$PWD/logic/CentStateManager.h
HEADERS += $$PWD/logic/SummaryController.h
HEADERS += $$PWD/logic/PowerSpectrumData.h
HEADERS += $$PWD/logic/PowerSpectrumFileReader.h
HEADERS += $$PWD/logic/SessionNotesLauncher.h
HEADERS += $$PWD/logic/controllers/CentMainWindowController.h

############################################## HELPERS HEADERS
HEADERS += $$PWD/helpers/UiHelpers.h

############################################# DATA CENTER HEADERS
HEADERS += $$PWD/dataCenter/BackupManager.h
HEADERS += $$PWD/dataCenter/CentPatientData.h
HEADERS += $$PWD/dataCenter/PatientDataCenterConfig.h
HEADERS += $$PWD/dataCenter/PatientDataCenter.h
HEADERS += $$PWD/dataCenter/PatientDataUnit.h

############################################## UI HEADERS
HEADERS += $$PWD/ui/pages/BaselineInfoPage.h
HEADERS += $$PWD/ui/pages/SimplePage.h
HEADERS += $$PWD/ui/pages/ChooseScenarioGroupPage.h
HEADERS += $$PWD/ui/pages/EvaluationPage.h
HEADERS += $$PWD/ui/pages/PatientQuestionaryPage.h
HEADERS += $$PWD/ui/pages/GamePage.h
HEADERS += $$PWD/ui/pages/SetupCapPage.h
HEADERS += $$PWD/ui/pages/SummaryPage.h
HEADERS += $$PWD/ui/pages/SessionFeedbackPage.h
HEADERS += $$PWD/ui/pages/GameSelectPage.h
HEADERS += $$PWD/ui/pages/TutorialPage.h

HEADERS += $$PWD/ui/AboutWidget.h
HEADERS += $$PWD/ui/EvaluationQuestionaryUi.h
HEADERS += $$PWD/ui/ComboBox.h
HEADERS += $$PWD/ui/PatientQuestionaryUi.h
HEADERS += $$PWD/ui/ConsoleOutput.h
HEADERS += $$PWD/ui/DebugOutputManager.h
HEADERS += $$PWD/ui/SettingsWidget.h
HEADERS += $$PWD/ui/SelectCapDriverDialog.h

############################################## CONTROLLERS HEADERS
HEADERS += $$PWD/logic/controllers/BaselinePageController.h
HEADERS += $$PWD/logic/controllers/BaselineInfoPageController.h
HEADERS += $$PWD/logic/controllers/ChooseScenarioGroupPageController.h
HEADERS += $$PWD/logic/controllers/EvaluationPageController.h
HEADERS += $$PWD/logic/controllers/FinishPageController.h
HEADERS += $$PWD/logic/controllers/PatientQuestionaryPageController.h
HEADERS += $$PWD/logic/controllers/GamePageController.h
HEADERS += $$PWD/logic/controllers/SetupCapPageController.h
HEADERS += $$PWD/logic/controllers/SummaryPageController.h
HEADERS += $$PWD/logic/controllers/SessionFeedbackPageController.h
HEADERS += $$PWD/logic/controllers/GameSelectPageController.h
HEADERS += $$PWD/logic/controllers/TutorialPageController.h
HEADERS += $$PWD/logic/controllers/WelcomePageController.h

############################################# WIDGETS HEADERS
HEADERS += $$PWD/widgets/BarChartItem.h
HEADERS += $$PWD/widgets/BarChartWidget.h
HEADERS += $$PWD/widgets/EEGPlot.h
HEADERS += $$PWD/widgets/ArraySeriesData.h
HEADERS += $$PWD/widgets/EEGMultiWidget.h
HEADERS += $$PWD/widgets/NameSurnameInfoWidget.h
HEADERS += $$PWD/widgets/MoodInfoWidget.h
HEADERS += $$PWD/widgets/MultiButtonWidget.h
HEADERS += $$PWD/widgets/ExcitementInfoWidget.h
HEADERS += $$PWD/widgets/MotivationInfoWidget.h
HEADERS += $$PWD/widgets/SleepInfoWidget.h
HEADERS += $$PWD/widgets/IInfoWidget.h
HEADERS += $$PWD/widgets/NoisePlot.h
HEADERS += $$PWD/widgets/MultiButtonWidgetBase.h
HEADERS += $$PWD/widgets/BaseLinePlot.h
HEADERS += $$PWD/widgets/PowerLevelBar.h
HEADERS += $$PWD/widgets/PowerBarHistoryPlot.h

############################################## ERROR HANDLING SOURCES
SOURCES += $$PWD/common/ErrorHandler.cpp

############################################## LOGIC SOURCES
SOURCES += $$PWD/logic/CentController.cpp
SOURCES += $$PWD/logic/CentGameManager.cpp
SOURCES += $$PWD/logic/CentGamePluginManager.cpp
SOURCES += $$PWD/logic/CentStateManager.cpp
SOURCES += $$PWD/logic/SummaryController.cpp
SOURCES += $$PWD/logic/PowerSpectrumData.cpp
SOURCES += $$PWD/logic/PowerSpectrumFileReader.cpp
SOURCES += $$PWD/logic/SessionNotesLauncher.cpp
SOURCES += $$PWD/logic/controllers/CentMainWindowController.cpp

############################################## HELPERS SOURCES
SOURCES += $$PWD/helpers/UiHelpers.cpp

############################################# DATA CENTER SOURCES
SOURCES += $$PWD/dataCenter/BackupManager.cpp
SOURCES += $$PWD/dataCenter/PatientDataCenter.cpp
SOURCES += $$PWD/dataCenter/PatientDataUnit.cpp

############################################## UI SOURCES
SOURCES += $$PWD/ui/pages/BaselineInfoPage.cpp
SOURCES += $$PWD/ui/pages/SimplePage.cpp
SOURCES += $$PWD/ui/pages/ChooseScenarioGroupPage.cpp
SOURCES += $$PWD/ui/pages/EvaluationPage.cpp
SOURCES += $$PWD/ui/pages/PatientQuestionaryPage.cpp
SOURCES += $$PWD/ui/pages/GamePage.cpp
SOURCES += $$PWD/ui/pages/SetupCapPage.cpp
SOURCES += $$PWD/ui/pages/SummaryPage.cpp
SOURCES += $$PWD/ui/pages/SessionFeedbackPage.cpp
SOURCES += $$PWD/ui/pages/GameSelectPage.cpp
SOURCES += $$PWD/ui/pages/TutorialPage.cpp

SOURCES += $$PWD/ui/AboutWidget.cpp
SOURCES += $$PWD/ui/EvaluationQuestionaryUi.cpp
SOURCES += $$PWD/ui/ComboBox.cpp
SOURCES += $$PWD/ui/PatientQuestionaryUi.cpp
SOURCES += $$PWD/ui/ConsoleOutput.cpp
SOURCES += $$PWD/ui/DebugOutputManager.cpp
SOURCES += $$PWD/ui/SettingsWidget.cpp
SOURCES += $$PWD/ui/SelectCapDriverDialog.cpp

############################################## CONTROLLERS SOURCES
SOURCES += $$PWD/logic/controllers/BaselinePageController.cpp
SOURCES += $$PWD/logic/controllers/BaselineInfoPageController.cpp
SOURCES += $$PWD/logic/controllers/PatientQuestionaryPageController.cpp
SOURCES += $$PWD/logic/controllers/ChooseScenarioGroupPageController.cpp
SOURCES += $$PWD/logic/controllers/EvaluationPageController.cpp
SOURCES += $$PWD/logic/controllers/FinishPageController.cpp
SOURCES += $$PWD/logic/controllers/GamePageController.cpp
SOURCES += $$PWD/logic/controllers/SetupCapPageController.cpp
SOURCES += $$PWD/logic/controllers/SummaryPageController.cpp
SOURCES += $$PWD/logic/controllers/SessionFeedbackPageController.cpp
SOURCES += $$PWD/logic/controllers/GameSelectPageController.cpp
SOURCES += $$PWD/logic/controllers/TutorialPageController.cpp
SOURCES += $$PWD/logic/controllers/WelcomePageController.cpp

############################################# WIDGETS SOURCES
SOURCES += $$PWD/widgets/BarChartItem.cpp
SOURCES += $$PWD/widgets/BarChartWidget.cpp
SOURCES += $$PWD/widgets/EEGPlot.cpp
SOURCES += $$PWD/widgets/ArraySeriesData.cpp
SOURCES += $$PWD/widgets/EEGMultiWidget.cpp
SOURCES += $$PWD/widgets/NameSurnameInfoWidget.cpp
SOURCES += $$PWD/widgets/MoodInfoWidget.cpp
SOURCES += $$PWD/widgets/MultiButtonWidget.cpp
SOURCES += $$PWD/widgets/ExcitementInfoWidget.cpp
SOURCES += $$PWD/widgets/MotivationInfoWidget.cpp
SOURCES += $$PWD/widgets/SleepInfoWidget.cpp
SOURCES += $$PWD/widgets/NoisePlot.cpp
SOURCES += $$PWD/widgets/MultiButtonWidgetBase.cpp
SOURCES += $$PWD/widgets/BaseLinePlot.cpp
SOURCES += $$PWD/widgets/PowerLevelBar.cpp
SOURCES += $$PWD/widgets/PowerBarHistoryPlot.cpp
