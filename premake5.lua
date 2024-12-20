workspace "OcAssistant"
    architecture "x64"
    configurations { "Debug", "Release" }

project "OcAssistant"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "src/**.h",
        "src/**.cpp",
        "external/glad/include/**.h",
        "external/glad/src/**.c",
    }

    includedirs {
        "src",
        "external/glad/include",
        "external/glfw/include",
    }

    links { 
        "glfw", 
        "opengl32",
    }

    libdirs {
        "external/nvapi/x86/nvapi.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "GLFW"
    kind "StaticLib"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files { "external/glfw/src/**.c", "external/glfw/include/GLFW/**.h" }

    includedirs { "external/glfw/include" }

    filter "system:windows"
        systemversion "latest"
        defines { "_GLFW_WIN32" }
