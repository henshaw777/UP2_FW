@echo off
SetLocal EnableDelayedExpansion EnableExtensions

:: Assign initial values
set thisscript=%0
set exitCode=0
set "Build_Flags= "
set "Stitch_Flags= "
set Arch=X64
set SkipUsageFlag=FALSE
set FabId=B
set BoardId=MN
set buildthread=
cd ..
set WORKSPACE=%CD%
if %WORKSPACE:~-1%==\ (
  set WORKSPACE=%WORKSPACE:~0,-1%
)
echo %WORKSPACE%
set CORE_PATH=%WORKSPACE%\edk2
set EDK_TOOLS_PATH=%CORE_PATH%\BaseTools
set PLATFORM_PATH=edk2-platforms\Platform\BroxtonPlatformPkg
set SILICON_PATH=edk2-platforms\Silicon\BroxtonSoC
set STITCH_PATH=%PLATFORM_PATH%\Common\Tools\Stitch

set Minnow_RVP=MINN

:: Parse Optional arguments
:OptLoop
if /i "%~1"=="" goto Usage
if /i "%~1"=="/?" goto Usage

::Build Flags
if /i "%~1"=="/ia32" (
    set Arch=IA32
    set Build_Flags=%Build_Flags% /ia32
    shift
    goto OptLoop
)
if /i "%~1"=="/x64" (
    set Arch=X64
    set Build_Flags=%Build_Flags% /x64
    shift
    goto OptLoop
)

if /i "%~1"=="/vs08" (
    set Build_Flags=%Build_Flags% /vs08
    shift
    goto OptLoop
)
if /i "%~1"=="/vs10" (
    set Build_Flags=%Build_Flags% /vs10
    shift
    goto OptLoop
)
if /i "%~1"=="/vs12" (
    set Build_Flags=%Build_Flags% /vs12
    shift
    goto OptLoop
)
if /i "%~1"=="/vs13" (
    set Build_Flags=%Build_Flags% /vs13
    shift
    goto OptLoop
)
if /i "%~1"=="/vs15" (
    set Build_Flags=%Build_Flags% /vs15
    shift
    goto OptLoop
)
if /i "%~1"=="/FspW" (
    set Build_Flags=%Build_Flags% /FspW
    shift
    goto OptLoop
)
if /i "%~1"=="/A" (
    set FabId=A
    set Build_Flags=%Build_Flags% /A
    shift
    goto OptLoop
)
if /i "%~1"=="/B" (
    set FabId=B
    set Build_Flags=%Build_Flags% /B
    shift
    goto OptLoop
)
if /i "%~1"=="/C" (
    set FabId=C
    set Build_Flags=%Build_Flags% /C
    shift
    goto OptLoop
)
if /i "%~1"=="/D" (
    set FabId=D
    set Build_Flags=%Build_Flags% /D
    shift
    goto OptLoop
)
if /i "%~1"=="/MN" (
    set BoardId=MN
    set Build_Flags=%Build_Flags% /MN
    shift
    goto OptLoop
)
if /i "%~1"=="/BG" (
    set BoardId=BG
    set Build_Flags=%Build_Flags% /BG
    shift
    goto OptLoop
)

if /i "%~1"=="/AG" (
    set BoardId=AG
    set Build_Flags=%Build_Flags% /AG
    shift
    goto OptLoop
)

if /i "%~1"=="/MX" (
    set BoardId=MX
    set Build_Flags=%Build_Flags% /MX
    shift
    goto OptLoop
)

if /i "%~1"=="/LH" (
    set BoardId=LH
    set Build_Flags=%Build_Flags% /LH
    shift
    goto OptLoop
)

if /i "%~1"=="/UP" (
    set BoardId=UP
    set Build_Flags=%Build_Flags% /UP
    shift
    goto OptLoop
)

if /i "%~1"=="/m" (
    set buildthread=/m
    echo.
    shift
    goto OptLoop
)

if /i "%~1"=="/L" (
    set Stitch_Flags=L
    shift
    goto OptLoop
)

if /i "%~1"=="/j" (
    set Build_Flags=%Build_Flags% /J
    shift
    goto OptLoop
)

:: Require 2 input parameters
if "%~2"=="" (
   echo. & echo -- ERROR: Not Enough Arguments Provided
   echo -- Please review the Help screen %thisscript% "/?" -- & echo.
   goto exit
)

:: Assign required arguments
set Platform_Type=%~1
set Build_Target=%~2

:: Build BIOS
echo ================================================================================
echo Build_IFWI:  Calling BIOS build Script...
echo.

echo - call BuildBxtBios.bat %buildthread% %Build_Flags% %Platform_Type% %Build_Target%
call %WORKSPACE%\%PLATFORM_PATH%\BuildBxtBios.bat %buildthread% %Build_Flags% %Platform_Type% %Build_Target%
if ErrorLevel 1 (
    echo echo  -- Error Building BIOS  & echo.
    set exitCode=1
    goto exit
)

echo ================================================================================
echo.

call Conf\BiosId.bat
echo BIOS_ID=%BIOS_ID%

:: Set the Board_Id, Build_Type, Version_Major, and Version_Minor environment variables
find /v "#" Conf\BiosId.env > ver_strings
for /f "tokens=1,3" %%i in (ver_strings) do set %%i=%%j
del /f/q ver_strings >nul

:: Translate Release Build Type
if "%BUILD_TYPE%"=="R" set BUILD_TYPE=R

set BIOS_Name=%BOARD_ID%%BOARD_REV%_%Arch%_%BUILD_TYPE%_%VERSION_MAJOR%_%VERSION_MINOR%

:: Start Integration process
echo ================================================================================
echo Build_IFWI:  Calling IFWI Stitching Script...
echo.
echo BIOS ROM input:  %BIOS_Name%
echo.
pushd %STITCH_PATH%
   echo  - call IFWIStitch_Simple.bat %WORKSPACE%\%STITCH_PATH%\%BIOS_Name% %FabId% %BoardId% %Stitch_Flags%
   call IFWIStitch_Simple.bat %WORKSPACE%\%STITCH_PATH%\%BIOS_Name% %FabId% %BoardId% %Stitch_Flags%
   @echo off
popd
if ErrorLevel 1 (
    echo echo  -- Error Stitching %BIOS_Name% & echo.
    set exitCode=1
)
echo.
echo Build_IFWI is finished.
echo The final IFWI file is located in %WORKSPACE%\%PLATFORM_PATH%\Common\Tools\Stitch\
echo ======================================================================


goto Exit

:Usage
if /i "%SkipUsageFlag%" == "TRUE" goto Exit
echo Script to build BIOS firmware and stitch the entire IFWI.
echo.
echo Usage: %thisscript% [options] ^<PlatformType^> ^<BuildTarget^>
echo.
echo.    /?       Display this help text
echo     /j       Log a copy of the build output to EDK2.log
echo     /c       CleanAll before building
echo     /m 	  Set the build thread count to number of processors
echo     /FspW    Build FSP and FSP Wrapper
echo     /x64     Set Arch to X64 (default)
echo     /vs08    Set compiler to VisualStudio 2008
echo     /vs10    Set compiler to VisualStudio 2010
echo     /vs12    Set compiler to VisualStudio 2012
echo     /vs13    Set compiler to VisualStudio 2013
echo     /vs15    Set compiler to VisualStudio 2015
echo.
echo     Platform Types:   %Minnow_RVP%
echo     MINN - Minnow (ApolloLake)
echo.
echo     Build Targets:    Release, Debug
echo.
set exitCode=1

:Exit
(
  EndLocal
  set Arch=%Arch%
  exit /b %exitCode%
)
