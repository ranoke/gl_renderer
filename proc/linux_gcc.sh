#!/bin/bash

rm -rf bin
mkdir bin
cd bin

proj_name=gl_raytracing
proj_root_dir=$(pwd)/../

flags=(
  -std=c++11 -Wl,--no-as-needed -lGLEW -lglfw -ldl -lGL -lX11 -pthread -lXi
)

inc=(
	-I ../third_party/include/
)

src=(
	../source/main.cpp
  ../source/window.cpp
  ../source/gfx.cpp
)

g++ -g -w ${inc[*]} ${src[*]} ${flags[*]} -lm -o ${proj_name}

cd ..
