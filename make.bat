@echo off

if "%1" == "" goto release
if "%1" == "release" goto release
if "%1" == "debug" goto debug
if "%1" == "clean" goto clean

@echo "Usage: make.bat [release|debug`|clean]"
goto end

:release
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
goto end

:debug
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
goto end

:clean:
rmdir /s /q build
del /q /f *.exe
del /q /f *.pdb
del /q /f *.log

:end
