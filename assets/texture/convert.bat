@echo off

for %%f in (*.png) do (
    echo Generating %%~nf.h
    xxd -i "%%f" > "%%~nf.h"
)

echo Done.
pause
