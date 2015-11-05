REM  -----------------------------------
REM  copyright: BLStream 2012
REM  author:    robert.rabenel@blstream.com
REM  www:       http://www.blstream.com 
REM  -----------------------------------

REM clean source directories before copying them to public repo
git reset --hard
if not %errorlevel%==0 goto errorexit
call copy_sources_for_publish.bat
if not %errorlevel%==0 goto errorexit

@echo on
REM you need VS 2010 to run this script
set VSVARSALL="%VS100COMNTOOLS%vsvars32.bat"
if not exist %VSVARSALL% exit /b 255

pushd ..\
REM you need %OPENVIBE_SRC% variable set and pointing to directory with sources of openVibe
REM you can get it from here: svn://scm.gforge.inria.fr/svn/openvibe/trunk, 
REM This script was created based on svn://scm.gforge.inria.fr/svn/openvibe/tags/0.12.0
set OVAPPS=%OPENVIBE_SRC%\openvibe-applications\
if not exist %OVAPPS% exit /b 255

rd %OVAPPS%\acquisition-server\cent
mklink /J %OVAPPS%\acquisition-server\cent AcquisitionServer
if not %errorlevel%==0 goto errorexit

rd %OVAPPS%\designer\cent
mklink /J %OVAPPS%\designer\cent Designer
if not %errorlevel%==0 goto errorexit

popd
xcopy /Y .\win32-init_env_command.cmd %OPENVIBE_SRC%\scripts\win32-init_env_command.cmd

pushd %OPENVIBE_SRC%\scripts\

call %VSVARSALL%
if not %errorlevel%==0 goto errorexit
call win32-build.cmd
if not %errorlevel%==0 goto errorexit
popd

REM Copy binaries to output folder, where from Jenkins will archive them as artifacts
pushd ..\
rd /Q /S output
mkdir output
copy /Y AcquisitionServer\bin\OpenViBE-acquisition-server-dynamic.exe output\OpenViBEAcquisitionServerDynamic.exe
if not %errorlevel%==0 goto errorexit
copy /Y Designer\bin\OpenViBE-designer-dynamic.exe output\OpenViBEDesignerDynamic.exe
if not %errorlevel%==0 goto errorexit
REM this you need to get from StarLab yourself, BLStream does not redistribute it.
copy /Y EnobioAPI\lib\VC2010\EnobioAPI.dll output\EnobioAPI.dll
if not %errorlevel%==0 goto errorexit
copy /Y Enobio3GAPI\libs\VC2010\Enobio3GAPI.dll output\Enobio3GAPI.dll
if not %errorlevel%==0 goto errorexit
copy /Y Enobio3GAPI\libs\VC2010\WinBluetoothAPI.dll output\WinBluetoothAPI.dll
if not %errorlevel%==0 goto errorexit

popd
pushd build\
call publish_opensource_code.bat
if not %errorlevel%==0 goto errorexit

popd
goto :eof

:errorexit
popd
exit /b 1
