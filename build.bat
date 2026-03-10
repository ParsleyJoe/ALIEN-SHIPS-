@echo off
set BUILD_DIR=build\

cmake --build %BUILD_DIR% || exit /b

%BUILD_DIR%mygame.exe
