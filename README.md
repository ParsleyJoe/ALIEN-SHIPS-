# ALIEN SHIPS!

An endless **bullet hell** shooter written in **C++** using **Raylib**.

🎥 **Gameplay**

https://github.com/user-attachments/assets/691fdd50-bfa5-4342-9d67-b66da3024060

> **Note:** The gameplay recording is a little choppy because it was captured on low-end hardware.

---

## 📥 Download
Download on **itch.io**
 - https://parsleyjoe.itch.io/alien-ships

# 🔨 Building

## Requirements
* Cmake
* Ninja
* C++17 or newer compiler

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
