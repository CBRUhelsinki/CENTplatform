@echo off
set VSVARSALL="%VS100COMNTOOLS%vsvars32.bat"
if not exist %VSVARSALL% exit /b 255

pushd ..\..\

if exist cent_vrpn goto fetch_vrpn_repo
git clone git+ssh://git/project/cent_vrpn.git cent_vrpn
if not %errorlevel%==0 goto errorexit
goto vrpn_repo_ready

:fetch_vrpn_repo
pushd cent_vrpn
git fetch origin
if not %errorlevel%==0 goto giterrorexit
git reset --hard origin/master
if not %errorlevel%==0 goto giterrorexit
popd

:vrpn_repo_ready
if not exist cent_vrpn\vrpn_folder.txt goto errorexit
set /p VRPN_SUBFOLDER=< cent_vrpn\vrpn_folder.txt
REM set the variable that is used by Application and Games builds
cd > current_folder.txt
set /p CD_CURRENT=< current_folder.txt
set VRPN=%CD_CURRENT%\cent_vrpn\%VRPN_SUBFOLDER%
if not exist %VRPN% goto errorexit

REM copy latest successfull artifacts
xcopy /S /I /Y "vrpn_artifacts\%VRPN_SUBFOLDER%\pc_win32\*" "%VRPN%\pc_win32"

popd
pushd ..\

REM recheck VRPN so to be sure there is not a relative path
if not exist %VRPN% goto errorexit

REM get sdk submodule
REM git submodule update --init
REM if not %errorlevel%==0 goto errorexit

call SDK\build\set_cent_version_vars.bat
pushd build
call generate_version.bat
popd

call %VSVARSALL%

if not %errorlevel%==0 goto errorexit
call qmake -tp vc -r CENTSystem.pro
if not %errorlevel%==0 goto errorexit

msbuild.exe CENTApplication\CENTapplicationd.vcxproj /p:Configuration=Release,Platform=Win32
if not %errorlevel%==0 goto errorexit
msbuild.exe CENTApplication\CENTapplicationd.vcxproj /p:Configuration=Debug,Platform=Win32
if not %errorlevel%==0 goto errorexit

msbuild.exe CENTApplication\UnitTest\CENTapplication_testd.vcxproj /p:Configuration=Release,Platform=Win32
if not %errorlevel%==0 goto errorexit
msbuild.exe CENTApplication\UnitTest\CENTapplication_testd.vcxproj /p:Configuration=Debug,Platform=Win32
if not %errorlevel%==0 goto errorexit

msbuild.exe MappingTool\MappingToold.vcxproj /p:Configuration=Release,Platform=Win32
if not %errorlevel%==0 goto errorexit
msbuild.exe MappingTool\MappingToold.vcxproj /p:Configuration=Debug,Platform=Win32
if not %errorlevel%==0 goto errorexit

popd
goto :eof

:giterrorexit
popd
:errorexit
popd
exit /b 1