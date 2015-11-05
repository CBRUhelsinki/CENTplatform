@echo on

set x=%1
set y=%2
set source=%x:/=\%
set destination=%y:/=\%
set target=%3
set PATH=%QTDIR%\bin;%PATH%

copy %source%\vld\bin\Win32\vld_x86.dll %destination%\vld_x86.dll
copy %source%\vld\bin\Win32\dbghelp.dll %destination%\dbghelp.dll
copy %source%\vld\vld.ini %destination%\vld.ini

pushd %source%\qwt\bin\ 
echo %cd%
call get_latest_binaries.bat
popd

copy %source%\qwt\bin\qwtd.dll %destination%\qwtd.dll
copy %source%\qwt\bin\qwt.dll %destination%\qwt.dll

chdir %destination%

call %target%
if not %errorlevel%==0 goto errorexit

more < vld.log

goto :eof

:errorexit
echo ERROR: Tests failed for %target%
exit /b 1
