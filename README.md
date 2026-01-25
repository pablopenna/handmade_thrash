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

All in one:

```sh
rm -rf build && cmake -S . -B build && cmake --build build && ./build/Debug/handmade_thrash
```

## Procedures

### Undo submodule changes

```sh
git submodule deinit -f .
git submodule update --init
```

## Known issues


## Troubleshooting

### Monitoring memory leaks

``` sh
valgrind --leak-check=full ./build/Debug/handmade_thrash 
```

## Resources
* Handmade Penguin - https://davidgow.net/handmadepenguin
* SDL + CMake setup - https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md, https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md
* SDL Linux Readme - https://wiki.libsdl.org/SDL3/README-linux#build-dependencies, https://wiki.libsdl.org/SDL2/README-cmake
