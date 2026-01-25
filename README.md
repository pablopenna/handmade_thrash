# handmade_thrash
Project made to follow Handmade Hero + Handmade Penguin

## Requirements
* Linux 
* CMake
* ~SDL 2/3~ included as a git submodule

## Build
(First time only) Initialize SDL submodule

```sh
git submodule update --init --recursive
```

Setup the build system
```sh
cmake -S . -B build
cmake --build build
```

## Monitoring memory leaks

``` sh
valgrind --leak-check=full ./build/Debug/handmade_thrash 
```

## Troubleshooting
### Segmentation fault when resizing window when using mmap to allocate pixels

Seems to be related to memory leak below. Happens in SDL2 as well.

### Ram usage increase slowly but constantly
There seems to be a memory leak in SDL3 for Linux in some scenarios where Wayland is involved

https://github.com/libsdl-org/SDL/issues/10273

This does not seem to happen in SDL2

## Resources
* Handmade Penguin - https://davidgow.net/handmadepenguin
* SDL + CMake setup - https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md, https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md
* SDL Linux Readme - https://wiki.libsdl.org/SDL3/README-linux#build-dependencies, https://wiki.libsdl.org/SDL2/README-cmake
