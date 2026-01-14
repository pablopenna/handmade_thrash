#!/bin/bash
mkdir -p ./build
pushd ./build
c++ ../src/handmade.cpp -o handmade_thrash -g
popd
