#!/bin/bash

rm -rf bin
mkdir bin
cd bin

proj_name=gl_renderer
proj_root_dir=$(pwd)/../

flags=(
  -std=c++17 -Wl,--no-as-needed -lGLEW -lglfw -ldl -lGL -lX11 -pthread -lXi
)

inc=(
  -I ../source/
	-I ../third_party/include/
  -I ../third_party/imgui/
  -I ../third_party/flecs/
)

src=(
	../source/main.cpp
  ../source/window.cpp
  ../source/gfx/gfx.cpp
  ../source/gfx/gfx_utils.cpp
  ../source/gui/gui.cpp
  ../source/utils/stb_image.cpp
  ../source/utils/stb_perlin.cpp
  ../source/utils/stb_leakcheck.cpp
  ../source/utils/stb_include.cpp
  ../source/utils/tiny_obj_loader.cpp

  # imgui
  ../third_party/imgui/imgui_demo.cpp
  ../third_party/imgui/imgui_draw.cpp
  ../third_party/imgui/imgui_tables.cpp
  ../third_party/imgui/imgui_widgets.cpp
  ../third_party/imgui/imgui.cpp

  # flecs
  ../third_party/flecs/flecs.c
)

defines=(
  -D GLFW_INCLUDE_NONE
  #-D STB_LEAKCHECK_SHOWALL
)

g++ -g -w ${defines[*]} ${inc[*]} ${src[*]} ${flags[*]} -lm -o ${proj_name}

cd ..
