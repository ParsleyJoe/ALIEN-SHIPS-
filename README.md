# ALIEN SHIPS!

Endless bullet hell game, made in C++, Raylib.

![Gif of working game](https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExcWdmajBsaWcwYjBmN2Q3cjJuZTI3dWdncDIxYzE1bWUzdXVteHV3MSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/R1KvwYjxcx6BZFV0Dr/giphy.gif)

*Resolution is bad; please download and try the game*

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
