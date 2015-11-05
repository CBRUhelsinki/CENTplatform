REM this script works on an assumption that current cent_openvibe.git is clean, no build artifacts

pushd ..

REM update public repository with latest sources
rd /Q /S cent-openvibe-public
git clone github.com:blstream/cent-openvibe-public
if not %errorlevel%==0 goto errorexit
xcopy /E /Y AcquisitionServer cent-openvibe-public\AcquisitionServer\
if not %errorlevel%==0 goto errorexit
xcopy /E /Y build cent-openvibe-public\build\
if not %errorlevel%==0 goto errorexit
xcopy /E /Y Designer cent-openvibe-public\Designer\
if not %errorlevel%==0 goto errorexit

popd
goto :eof

:errorexit
popd
echo "Error copying opensource code!"
exit /b 1
