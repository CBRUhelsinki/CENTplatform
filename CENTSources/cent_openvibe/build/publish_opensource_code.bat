REM This script works on an assumption that there is already a public repo cloned 
REM it should be cloned into cent-openvibe-public in the upper directory level 

pushd ..\cent-openvibe-public\
if not %errorlevel%==0 goto errorexit

git add *
if not %errorlevel%==0 goto errorexit

git commit -m "Cent OpenVibe apps commiting latest sources from build: %BUILD_NUMBER%"
if not %errorlevel%==0 goto errorexit

git push origin master
if not %errorlevel%==0 goto errorexit

popd
goto :eof

:errorexit
popd
echo "Error publishing opensource code!"
exit /b 1
