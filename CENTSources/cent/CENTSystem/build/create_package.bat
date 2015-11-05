IF "%1"=="debug" (
	set BUILD_MODE=-dDebug
	set post_fix="d"
) ELSE (
	set BUILD_MODE=
	set post_fix=""
)
REM cleranup so that we do not include output from previous build
del /F /Q ..\OVScenarios\release\4Channels\CentScenarios4.wxs
heat dir ..\OVScenarios\release\4Channels -cg CentScenarios4 -var sys.SOURCEFILEDIR -dr CHANNELS_4 -gg -srd -o ..\OVScenarios\release\4Channels\CentScenarios4.wxs

del /F /Q ..\OVScenarios\release\8Channels\CentScenarios8.wxs
heat dir ..\OVScenarios\release\8Channels -cg CentScenarios8 -var sys.SOURCEFILEDIR -dr CHANNELS_8 -gg -srd -o ..\OVScenarios\release\8Channels\CentScenarios8.wxs

heat dir ..\files\Tutorials -cg CentGamesTutorials -var sys.SOURCEFILEDIR -dr PLUGINS -gg  -o ..\files\Tutorials\CentGamesTutorials.wxs

candle %BUILD_MODE% ..\OVScenarios\release\4Channels\CentScenarios4.wxs ..\OVScenarios\release\8Channels\CentScenarios8.wxs ..\files\Tutorials\CentGamesTutorials.wxs CENTInstaller.wxs  CentOVDirDlg.wxs CentWixUi_InstallDir.wxs CentBrowseDlg.wxs CentDataDirDlg.wxs -out output\obj\

light -ext WixUIExtension.dll output\obj\CentScenarios4.wixobj output\obj\CentScenarios8.wixobj output\obj\CentGamesTutorials.wixobj output\obj\CENTInstaller.wixobj  output\obj\CentOVDirDlg.wixobj output\obj\CentWixUi_InstallDir.wixobj output\obj\CentBrowseDlg.wixobj output\obj\CentDataDirDlg.wixobj -o output\CENTInstaller%post_fix%.msi
REM msiexec /I output\CENTInstaller.msi /l*v output\log.txt