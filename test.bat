@echo off
setlocal

cd /D "%~dp0"

if not exist build (exit)

pushd build 
call Debug\Tests.exe
if errorlevel 1 (
    echo ERROR: Run Failed! 
    popd
    exit /b 1
)
popd

endlocal
