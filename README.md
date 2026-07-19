# ALIEN SHIPS!

Endless bullet hell game, made in C++, Raylib.

https://github.com/user-attachments/assets/691fdd50-bfa5-4342-9d67-b66da3024060

*Recording is laggy as my pc can't handle OBS*

## Download
https://parsleyjoe.itch.io/alien-ships

# Building

### Windows

 - Release
    ```
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DPRODUCTION_BUILD=ON
    cmake --build build/
    ./build/mygame.exe
    ```
 - Debug
    ```
    cmake -B build -G Ninja
    cmake --build build/
    ./build/mygame.exe
    ```
