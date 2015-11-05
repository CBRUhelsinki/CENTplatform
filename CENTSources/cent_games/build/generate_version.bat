@echo off

IF "%BUILD_NUMBER%" == "" goto default_build_num
set CENT_BUILD_NUMBER=%BUILD_NUMBER%
goto build_number_set

:default_build_num
set CENT_BUILD_NUMBER=0

:build_number_set
echo current build number is %CENT_BUILD_NUMBER%


echo Generating version.h

echo #define CENT_VERSION_MAJOR %CENT_VERSION_MAJOR% >..\common\version.h
echo #define CENT_VERSION_MINOR %CENT_VERSION_MINOR% >>..\common\version.h
echo #define CENT_BUILD_NUMBER  %CENT_BUILD_NUMBER%  >>..\common\version.h
echo #define CENT_VERSION_STRING "%CENT_VERSION_MAJOR%.%CENT_VERSION_MINOR%.%CENT_BUILD_NUMBER%.0\0" >> ..\common\version.h

echo version.h generated

echo Version number: %CENT_VERSION_MAJOR%.%CENT_VERSION_MINOR%.%CENT_BUILD_NUMBER%.0

exit /b 0
