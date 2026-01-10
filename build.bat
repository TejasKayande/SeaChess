@echo off
setlocal

cd /D "%~dp0"

if not exist build (
   mkdir build
   pushd build
   call cmake ..
   if errorlevel 1 (
       echo ERROR: cmake configuration failed!
       popd
       exit /b 1
   )
   popd
)

pushd build 
call cmake --build . --parallel --config Debug
if errorlevel 1 (
    echo ERROR: build failed!
    popd
    exit /b 1
)
popd

endlocal
