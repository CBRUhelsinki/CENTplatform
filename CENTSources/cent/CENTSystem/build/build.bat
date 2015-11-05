#echo off
call buildCENT.bat
if not %errorlevel%==0 goto errorexit
call buildMappingToolPackage.bat
if not %errorlevel%==0 goto errorexit

goto :eof

:errorexit
popd
exit /b 1