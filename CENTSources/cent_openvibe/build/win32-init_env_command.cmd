@echo off

SET OpenViBE_base=%CD%\..
SET OpenViBE_percent=%%

REM ########################################################################################################################
REM ## GLOBAL
REM
REM ## this variable sets the branch for everything
SET OpenViBE_default_branch=
REM
REM ########################################################################################################################
REM ## GENERAL
REM
REM ## these variables SET each project branch
SET OpenViBE_branch=
SET OpenViBE_kernel_branch=
SET OpenViBE_toolkit_branch=
SET OpenViBE_documentation_branch=
SET OpenViBE_scenarios_branch=
REM
REM ########################################################################################################################
REM ## MODULES
REM
REM ## these variables SET each module branch
SET OpenViBE_module_automaton_branch=
SET OpenViBE_module_ebml_branch=
SET OpenViBE_module_fs_branch=
SET OpenViBE_module_socket_branch=
SET OpenViBE_module_stream_branch=
SET OpenViBE_module_system_branch=
SET OpenViBE_module_xml_branch=
REM
REM ########################################################################################################################
REM ## APPLICATIONS
REM
REM ## these variables SET each application branch
SET OpenViBE_application_acquisition_server_branch=cent
SET OpenViBE_application_designer_branch=cent
SET OpenViBE_application_id_generator_branch=
SET OpenViBE_application_plugin_inspector_branch=
SET OpenViBE_application_vr_demo_branch=
SET OpenViBE_application_vrpn_simulator_branch=
SET OpenViBE_application_ssvep_demo_branch=
REM
REM ########################################################################################################################
REM ## PLUGINS
REM
REM ## these variables SET each plugin package branch
SET OpenViBE_plugin_acquisition_branch=
SET OpenViBE_plugin_classification_branch=
SET OpenViBE_plugin_classification_gpl_branch=
SET OpenViBE_plugin_feature_extraction_branch=
SET OpenViBE_plugin_file_io_branch=
SET OpenViBE_plugin_local_branch=
SET OpenViBE_plugin_matlab_branch=
SET OpenViBE_plugin_samples_branch=
SET OpenViBE_plugin_signal_processing_branch=
SET OpenViBE_plugin_signal_processing_gpl_branch=
SET OpenViBE_plugin_simple_visualisation_branch=
SET OpenViBE_plugin_stimulation_branch=
SET OpenViBE_plugin_stream_codecs_branch=
SET OpenViBE_plugin_streaming_branch=
SET OpenViBE_plugin_tools_branch=
SET OpenViBE_plugin_turbofieldtrip_branch=
SET OpenViBE_plugin_vrpn_branch=
REM
REM ########################################################################################################################

REM ########################################################################################################################

if "%OpenViBE_default_branch%" == "" SET OpenViBE_default_branch=trunc

REM ########################################################################################################################

if "%OpenViBE_branch%" == "" SET OpenViBE_branch=%OpenViBE_default_branch%
if "%OpenViBE_kernel_branch%" == "" SET OpenViBE_kernel_branch=%OpenViBE_default_branch%
if "%OpenViBE_toolkit_branch%" == "" SET OpenViBE_toolkit_branch=%OpenViBE_default_branch%
if "%OpenViBE_documentation_branch%" == "" SET OpenViBE_documentation_branch=%OpenViBE_default_branch%
if "%OpenViBE_scenarios_branch%" == "" SET OpenViBE_scenarios_branch=%OpenViBE_default_branch%

REM ########################################################################################################################

SET OpenViBE=%OpenViBE_base%\openvibe\%OpenViBE_branch%
SET OpenViBE_kernel=%OpenViBE_base%\openvibe-kernel-omk\%OpenViBE_kernel_branch%
SET OpenViBE_toolkit=%OpenViBE_base%\openvibe-toolkit\%OpenViBE_toolkit_branch%
SET OpenViBE_documentation=%OpenViBE_base%\openvibe-documentation\%OpenViBE_documentation_branch%
SET OpenViBE_scenarios=%OpenViBE_base%\openvibe-scenarios\%OpenViBE_scenarios_branch%
SET OpenViBE_cmake_modules=%OpenViBE_base%\cmake-modules

REM ########################################################################################################################

SET OpenViBE_modules=%OpenViBE_base%\openvibe-modules
for /D %%s in (%OpenViBE_modules%/*) do (
	set OpenViBE_project_name=%%~ns
	set OpenViBE_project_name=!OpenViBE_project_name:-=_!
	echo if "%%OpenViBE_module_!OpenViBE_project_name!_branch%%" == "" SET OpenViBE_module_!OpenViBE_project_name!_branch=%OpenViBE_default_branch% >> tmp_branch.cmd
	echo set OpenViBE_module_!OpenViBE_project_name!=%OpenViBE_modules%\%%~ns\%OpenViBE_percent%OpenViBE_module_!OpenViBE_project_name!_branch%OpenViBE_percent% >> tmp_branch.cmd
	call tmp_branch.cmd
	del tmp_branch.cmd
)

REM ########################################################################################################################

SET OpenViBE_applications=%OpenViBE_base%\openvibe-applications
for /D %%s in (%OpenViBE_applications%/*) do (
	set OpenViBE_project_name=%%~ns
	set OpenViBE_project_name=!OpenViBE_project_name:-=_!
	echo if "%%OpenViBE_application_!OpenViBE_project_name!_branch%%" == "" SET OpenViBE_application_!OpenViBE_project_name!_branch=%OpenViBE_default_branch% >> tmp_branch.cmd
	echo set OpenViBE_application_!OpenViBE_project_name!=%OpenViBE_applications%\%%~ns\%OpenViBE_percent%OpenViBE_application_!OpenViBE_project_name!_branch%OpenViBE_percent% >> tmp_branch.cmd
	call tmp_branch.cmd
	del tmp_branch.cmd
)

REM ########################################################################################################################

SET OpenViBE_plugins=%OpenViBE_base%\openvibe-plugins
for /D %%s in (%OpenViBE_plugins%/*) do (
	set OpenViBE_project_name=%%~ns
	set OpenViBE_project_name=!OpenViBE_project_name:-=_!
	echo if "%%OpenViBE_plugin_!OpenViBE_project_name!_branch%%" == "" SET OpenViBE_plugin_!OpenViBE_project_name!_branch=%OpenViBE_default_branch% >> tmp_branch.cmd
	echo set OpenViBE_plugin_!OpenViBE_project_name!=%OpenViBE_plugins%\%%~ns\%OpenViBE_percent%OpenViBE_plugin_!OpenViBE_project_name!_branch%OpenViBE_percent% >> tmp_branch.cmd
	call tmp_branch.cmd
	del tmp_branch.cmd
)

REM ########################################################################################################################

SET OpenViBE_externals=%OpenViBE_base%\openvibe-externals
for /D %%s in (%OpenViBE_externals%/*) do (
	set OpenViBE_project_name=%%~ns
	set OpenViBE_project_name=!OpenViBE_project_name:-=_!
	echo if "%%OpenViBE_external_!OpenViBE_project_name!_branch%%" == "" SET OpenViBE_external_!OpenViBE_project_name!_branch=%OpenViBE_default_branch% >> tmp_branch.cmd
	echo set OpenViBE_external_!OpenViBE_project_name!=%OpenViBE_externals%\%%~ns\%OpenViBE_percent%OpenViBE_external_!OpenViBE_project_name!_branch%OpenViBE_percent% >> tmp_branch.cmd
	call tmp_branch.cmd
	del tmp_branch.cmd
)

REM ########################################################################################################################

if exist "win32-dependencies.cmd" (
	call "win32-dependencies.cmd"
)

REM ########################################################################################################################

SET VSTOOLS=
SET VSCMake=

if exist "!VS90COMNTOOLS!\vsvars32.bat" (
	SET VSTOOLS=!VS90COMNTOOLS!\vsvars32.bat
	SET VSCMake=Visual Studio 9 2008
)

if exist %VS100COMNTOOLS%\vsvars32.bat (
	SET VSTOOLS=%VS100COMNTOOLS%\vsvars32.bat
	SET VSCMake=Visual Studio 10 2010
)

if exist "%VSTOOLS%" (
	call "%VSTOOLS%"
) else (
	echo ######################################################################################
	echo ##                                                                                  ##
	echo ##  WARNING : Microsoft Visual Studio Common tools initialisation script not found  ##
	echo ##                                                                                  ##
	echo ######################################################################################
)

REM ########################################################################################################################

SET INCLUDE=%OV_DEP_MSSDK%\include;%INCLUDE%
SET INCLUDE=%OV_DEP_BOOST%;%INCLUDE%

SET LIB=%OV_DEP_BOOST%\lib;%LIB%
SET LIB=%OV_DEP_MSSDK%\lib;%LIB%

SET PATH=%OV_DEP_MSSDK%\bin;%PATH%
SET PATH=%OV_DEP_BOOST%\bin;%PATH%
SET PATH=%OV_DEP_GTK%\bin;%PATH%
SET PATH=%OV_DEP_CMAKE%\bin;%PATH%
SET PATH=%OV_DEP_EXPAT%\bin;%PATH%
SET PATH=%OV_DEP_ITPP%\bin;%PATH%
SET PATH=%OV_DEP_OGRE%\bin\release;%OV_DEP_OGRE%\bin\debug;%PATH%
SET PATH=%OV_DEP_CEGUI%\bin;%PATH%
SET PATH=%OV_DEP_LUA%\lib;%PATH%
REM SET PATH=%OV_DEP_MATLAB%\bin\win32;%PATH%
SET PATH=%OV_DEP_ALUT%\lib;%PATH%
SET PATH=%OV_DEP_OPENAL%\libs\Win32;%PATH%
SET PATH=%OV_DEP_OGG%\win32\bin\debug;%OV_DEP_OGG%\win32\bin\release;%PATH%
SET PATH=%OV_DEP_VORBIS%\win32\bin\debug;%OV_DEP_VORBIS%\win32\bin\release;%PATH%

SET PATH=%PATH%;%LIB%;%LIBPATH%

SET PKG_CONFIG_PATH=%OV_DEP_GTK%\lib\pkgconfig

SET OpenViBE_build_order=OpenViBE_build_order.txt

REM ########################################################################################################################

del %OpenViBE_build_order% > NULL 2<&1
REM echo %OpenViBE% >> %OpenViBE_build_order%
REM echo %OpenViBE_application_id_generator% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_ebml% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_fs% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_socket% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_system% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_stream% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_xml% >> %OpenViBE_build_order%
REM echo %OpenViBE_module_automaton% >> %OpenViBE_build_order%
REM echo %OpenViBE_kernel% >> %OpenViBE_build_order%
REM echo %OpenViBE_toolkit% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_acquisition% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_classification% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_classification_gpl% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_feature_extraction% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_file_io% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_samples% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_signal_processing% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_signal_processing_gpl% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_simple_visualisation% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_stimulation% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_stream_codecs% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_streaming% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_tools% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_vrpn% >> %OpenViBE_build_order%
echo %OpenViBE_application_designer% >> %OpenViBE_build_order%
echo %OpenViBE_application_acquisition_server% >> %OpenViBE_build_order%
REM echo %OpenViBE_application_plugin_inspector% >> %OpenViBE_build_order%
REM echo %OpenViBE_application_vr_demo% >> %OpenViBE_build_order%
REM echo %OpenViBE_application_vrpn_simulator% >> %OpenViBE_build_order%
REM echo %OpenViBE_application_skeleton_generator% >> %OpenViBE_build_order%
REM echo %OpenViBE_application_ssvep_demo% >> %OpenViBE_build_order%

REM echo %OpenViBE_plugin_matlab% >> %OpenViBE_build_order%
REM echo %OpenViBE_plugin_turbofieldtrip% >> %OpenViBE_build_order%

REM echo %OpenViBE_scenarios% >> %OpenViBE_build_order%

REM echo %OpenViBE_documentation% >> %OpenViBE_build_order%

REM echo %OpenViBE_external_% >> %OpenViBE_build_order%
