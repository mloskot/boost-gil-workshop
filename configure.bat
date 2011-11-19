@echo off
REM ### CONFIGURATION BEGIN ###
set BRANCH=master
set BUILD_TYPE=Debug
set BUILD=D:\dev\boost\gil\_git\build-%BRANCH%
set SRC=D:\dev\boost\gil\_git\boost-gil-workshop
set BOOST=D:\dev\boost\_svn\trunk
set OSGEO4W=D:\OSGeo4W
REM set TIFF=D:\src\tiff-4.0.0beta7
set G="Visual Studio 11"
REM ### CONFIGURATION END   ###
IF NOT EXIST %BUILD% mkdir %BUILD%
IF EXIST %BRANCH%.sln del /Q /F %BRANCH%.sln
echo Configuring build for %BRANCH% in %BUILD%
cd %SRC% & ^
git checkout -q %BRANCH% & ^
cd %BUILD% & ^
cmake -G %G% ^
    -DBOOST_ROOT=%BOOST% ^
    -DOSGEO4W_ROOT=%OSGEO4W% ^
    -DTIFF_INCLUDE_DIR=%OSGEO4W%\include ^
    -DTIFF_LIBRARY=%OSGEO4W%\lib\libtiff_i.lib ^
    %SRC%