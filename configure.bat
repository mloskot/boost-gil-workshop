@ECHO OFF
REM *** YOUR CONFIGURATION ***
set GIL_GDAL_TEST_BUILD_DIR="..\build"
set BOOST_ROOT=G:\dev\boost\_svn\trunk
set OSGEO4W_ROOT=D:\OSGeo4W
REM ********************************

set PATH=%OSGEO4W_ROOT%\apps\gdal-dev\bin;%OSGEO4W_ROOT%\bin;%PATH%
set GDAL_DATA=%OSGEO4W_ROOT%\apps\gdal-dev\share\gdal

set GIL_GDAL_TEST_DIR=%CD%
cd %GIL_GDAL_TEST_BUILD_DIR%
cmake ^
    -DBOOST_ROOT=%BOOST_ROOT% ^
    -DOSGEO4W_ROOT=%OSGEO4W_ROOT% ^
    %GIL_GDAL_TEST_DIR%
cd %GIL_GDAL_TEST_DIR%
