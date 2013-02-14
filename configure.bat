@echo off
REM ### CONFIGURATION BEGIN ###
set BRANCH=master
set BUILD_TYPE=Debug
SET SRCDIR="%~dp0"
SET BLDDIR="%~dp0_build-%BRANCH%"
set BOOST=D:\dev\boost\_svn\trunk
set OSGEO4W=C:\OSGeo4W
REM set TIFF=D:\src\tiff-4.0.0beta7
set G="Visual Studio 11"
REM ### CONFIGURATION END   ###
IF NOT EXIST %BLDDIR% mkdir %BLDDIR%
IF EXIST %BRANCH%.sln del /Q /F %BRANCH%.sln
echo Configuring build for %BRANCH% in %BLDDIR%
cd %SRC% & ^
cd %BLDDIR% & ^
cmake -G %G% ^
    -DBOOST_ROOT=%BOOST% ^
    %SRCDIR%