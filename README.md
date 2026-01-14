# handmade_thrash
Project made to follow Handmade Hero + Handmade Penguin

## Requirements
* Linux 
* CMake
* SDL 3

## Build
(First time only) Get SDL libraries

```
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
```

Setup the build system
```
cmake -S . -B build
cmake --build build
```

## Resources
* Handmade Penguin - https://davidgow.net/handmadepenguin
* SDL + CMake setup - https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md
* SDL Linux Readme - https://wiki.libsdl.org/SDL3/README-linux#build-dependencies
