workspace "gl_renderer"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["imgui"] = "/third_party/imgui"
IncludeDir["flecs"] = "/third_party/flecs"


includedirs
{
  "/third_party/imgui",
  "/third_party/flecs",
}

project "gl_renderer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "source/main.cpp",
      "source/window.cpp",
      "source/gfx/gfx.cpp",
      "source/gfx/gfx_utils.cpp",
      "source/gui/gui.cpp",
      "source/utils/stb_image.cpp",
      "source/utils/stb_perlin.cpp",
      "source/utils/stb_leakcheck.cpp",
      "source/utils/stb_include.cpp",
      "source/utils/tiny_obj_loader.cpp",
    
      --imgui
      "third_party/imgui/imgui_demo.cpp",
      "third_party/imgui/imgui_draw.cpp",
      "third_party/imgui/imgui_tables.cpp",
      "third_party/imgui/imgui_widgets.cpp",
      "third_party/imgui/imgui.cpp",

      -- flecs
      "third_party/flecs/flecs.c",
    }

    includedirs
    {
      "source",
      "third_party/imgui",
      "%{IncludeDir.imgui}",
      "%{IncludeDir.flecs}"
    }

    defines
    {
	    "GLFW_INCLUDE_NONE", 
    }

    links 
    { 
      "glfw",
      "GLEW",
      "dl",
      "GL"
    }

    filter "system:windows"
      system "windows"
      links { "OpenGL32" }
      

    filter "system:linux"
      system "linux"
      linkoptions {"-pthread"}
      

    filter "configurations:Debug"
      defines { "DEBUG" }
      runtime "Debug"
      symbols "On"

    filter "configurations:Release"
      defines { "RDEBUG" }
      runtime "Release"
      optimize "On"

    filter "configurations:Dist"
      defines { "NDEBUG" }
      runtime "Release"
      optimize "On"


