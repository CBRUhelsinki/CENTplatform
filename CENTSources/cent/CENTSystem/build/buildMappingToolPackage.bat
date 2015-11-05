@echo on
pushd ..\..\

REM get Openvibe repo
if exist Openvibe goto fetch_sdk_repo
git clone git+ssh://git/project/cent_openvibe.git Openvibe
if not %errorlevel%==0 goto errorexit

:fetch_sdk_repo
pushd Openvibe
git fetch origin
if not %errorlevel%==0 goto errorexit
git reset --hard origin/master
if not %errorlevel%==0 goto errorexit
popd

if exist MappingToolPackage rmdir /S /Q MappingToolPackage
if exist MappingToolPackage.zip del /F /Q MappingToolPackage.zip

xcopy /S /I /Y Documentation\MappingTool\README.txt "MappingToolPackage\"
xcopy /S /I /Y Openvibe\AcquisitionServer\bin\QtCore4.dll "MappingToolPackage\"
xcopy /S /I /Y CENTSystem\output\MappingTool.exe "MappingToolPackage\"

CENTSystem\SDK\external_tools\7zip\7za a MappingToolPackage.zip MappingToolPackage


popd
echo %cd%
exit /b 0