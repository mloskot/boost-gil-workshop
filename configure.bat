@ECHO OFF
REM *** YOUR CONFIGURATION ***
set GIL_GDAL_TEST_BUILD_DIR="..\build"
set BOOST=G:\dev\boost\_svn\trunk
set OSGEO4W=D:\OSGeo4W
set TIFF=G:\src\tiff-4.0.0beta7
REM ********************************

set PATH=%OSGEO4W_ROOT%\apps\gdal-dev\bin;%OSGEO4W%\bin;%PATH%
set GDAL_DATA=%OSGEO4W%\apps\gdal-dev\share\gdal

set GIL_GDAL_TEST_DIR=%CD%
cd %GIL_GDAL_TEST_BUILD_DIR%
cmake ^
    -DBOOST_ROOT=%BOOST% ^
    -DOSGEO4W_ROOT=%OSGEO4W% ^
	-DTIFF_INCLUDE_DIR=%TIFF%\libtiff ^
	-DTIFF_LIBRARY=%TIFF%\libtiff\libtiff.lib ^
    %GIL_GDAL_TEST_DIR%
cd %GIL_GDAL_TEST_DIR%
