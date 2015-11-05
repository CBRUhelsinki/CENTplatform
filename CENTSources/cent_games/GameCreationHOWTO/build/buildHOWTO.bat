@echo off

pushd ..\..\

cd > current_folder.txt
set /p CD_CURRENT=< current_folder.txt

set GAME_PACKAGE_DIR=%CD_CURRENT%\GameTutorial

if exist %CD_CURRENT%\GameTutorial rmdir /S /Q %CD_CURRENT%\GameTutorial
if exist %CD_CURRENT%\GameCreationHOWTO.zip del /F /Q %CD_CURRENT%\GameCreationHOWTO.zip

xcopy /S /I /Y cent\CENTSystem\Common\CentDataTypes.h "%GAME_PACKAGE_DIR%\Common\"
xcopy /S /I /Y cent\CENTSystem\Common\Connect.cpp "%GAME_PACKAGE_DIR%\Common\"
xcopy /S /I /Y cent\CENTSystem\Common\Connect.h "%GAME_PACKAGE_DIR%\Common\"

xcopy /S /I /Y cent\CENTSystem\Common\Interfaces\ICentGamePlugin.h "%GAME_PACKAGE_DIR%\Common\Interfaces\"

popd 
pushd ..\

xcopy /S /I /Y Common\GamePluginConf.pri "%GAME_PACKAGE_DIR%\Common\"

xcopy /S /I /Y Common\simpleshapedrawer.cpp "%GAME_PACKAGE_DIR%\Common\"
xcopy /S /I /Y Common\simpleshapedrawer.h "%GAME_PACKAGE_DIR%\Common\"

xcopy /S /I /Y Common\quicktimer.h "%GAME_PACKAGE_DIR%\Common\"
xcopy /S /I /Y Common\quicktimer.cpp "%GAME_PACKAGE_DIR%\Common\"
xcopy /S /I /Y Common\quicktimerthread.h "%GAME_PACKAGE_DIR%\Common\"
xcopy /S /I /Y Common\quicktimerthread.cpp "%GAME_PACKAGE_DIR%\Common\"

xcopy /S /I /Y SimpleBallGamePlugin\SimpleBallGamePlugin.pro "%GAME_PACKAGE_DIR%\SimpleBallGamePlugin\"

xcopy /S /I /Y SimpleBallGamePlugin\src\*.* "%GAME_PACKAGE_DIR%\SimpleBallGamePlugin\src\"
xcopy /S /I /Y SimpleBallGamePlugin\resources\*.* "%GAME_PACKAGE_DIR%\SimpleBallGamePlugin\resources\"

xcopy /S /I /Y GameCreationHOWTO\GameConfiguration.pri "%GAME_PACKAGE_DIR%\SimpleBallGamePlugin\"
xcopy /S /I /Y GameCreationHOWTO\README.txt "%GAME_PACKAGE_DIR%\"

popd
pushd ..\..\

SDK\external_tools\7zip\7za a GameCreationHOWTO.zip GameTutorial


popd
exit /b 0