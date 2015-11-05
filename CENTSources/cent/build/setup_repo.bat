@echo on
pushd ..\..\

REM get sdk repo
if exist SDK goto fetch_sdk_repo
git clone git+ssh://git/project/cent_sdk.git SDK
if not %errorlevel%==0 goto errorexit
goto sdk_repo_ready

:fetch_sdk_repo
pushd SDK
git fetch origin
if not %errorlevel%==0 goto errorexit
git reset --hard origin/master
if not %errorlevel%==0 goto errorexit
popd

:sdk_repo_ready
call SDK\build\set_cent_version_vars.bat


REM get cent repo
if exist cent goto fetch_cent_repo
git clone git+ssh://git/project/cent.git cent
if not %errorlevel%==0 goto errorexit

:fetch_cent_repo
pushd cent
git fetch origin
if not %errorlevel%==0 goto errorexit
git reset --hard origin/master
if not %errorlevel%==0 goto errorexit
popd

if not exist Common mklink /J Common cent\CENTSystem\Common
if not %errorlevel%==0 goto errorexit
popd
echo %cd%
exit /b 0
:errorexit
exit /b 1