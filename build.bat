@echo off
set BUILD_DIR=build\ninja-debug\

cmake --build %BUILD_DIR% || exit /b

%BUILD_DIR%mygame.exe
