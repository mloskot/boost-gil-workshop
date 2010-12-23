@ECHO OFF
REM *** YOUR CONFIGURATION ***
set GIL_GDAL_TEST_BUILD_DIR="..\build"
set BOOST_ROOT="G:\dev\boost\_svn\trunk"
REM ********************************

set GIL_GDAL_TEST_DIR=%CD%
cd %GIL_GDAL_TEST_BUILD_DIR%
cmake -DBOOST_ROOT=%BOOST_ROOT% %GIL_GDAL_TEST_DIR%
cd %GIL_GDAL_TEST_DIR%
