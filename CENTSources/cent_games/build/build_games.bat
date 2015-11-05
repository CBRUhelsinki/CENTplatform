@echo off
set VSVARSALL="%VS100COMNTOOLS%vsvars32.bat"
if not exist %VSVARSALL% exit /b 255

call generate_version.bat

pushd ..\

call %VSVARSALL%
if not %errorlevel%==0 goto errorexit
call qmake -tp vc -r GamePlugins.pro
if not %errorlevel%==0 goto errorexit

msbuild.exe EmptyBallGamePlugin\EmptyBallGamePlugind.vcxproj /p:Configuration=Release,Platform=Win32
if not %errorlevel%==0 goto errorexit
msbuild.exe EmptyBallGamePlugin\EmptyBallGamePlugind.vcxproj /p:Configuration=Debug,Platform=Win32
if not %errorlevel%==0 goto errorexit

msbuild.exe SimpleBallGamePlugin\SimpleBallGamePlugind.vcxproj /p:Configuration=Release,Platform=Win32
if not %errorlevel%==0 goto errorexit
msbuild.exe SimpleBallGamePlugin\SimpleBallGamePlugind.vcxproj /p:Configuration=Debug,Platform=Win32
if not %errorlevel%==0 goto errorexit

msbuild.exe MediaGamePlugin\MediaGamePlugind.vcxproj /p:Configuration=Release,Platform=Win32
if not %errorlevel%==0 goto errorexit
msbuild.exe MediaGamePlugin\MediaGamePlugind.vcxproj /p:Configuration=Debug,Platform=Win32
if not %errorlevel%==0 goto errorexit

popd
call ..\GameCreationHOWTO\build\buildHOWTO.bat
if not %errorlevel%==0 goto errorexit

goto :eof

:errorexit
popd
exit /b 1