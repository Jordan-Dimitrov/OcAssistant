workspace "OcAssistant"
    architecture "x64"
    configurations { "Debug", "Release" }

project "OcAssistant"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "src/**.h",
        "src/**.cpp",
        "external/glad/include/**.h",
        "external/glad/src/**.c",
        "external/stb/stb_image.h",
        "external/stb/stb_image_write.h",
    }

    includedirs {
        "src",
        "external/glad/include",
        "external/glfw/include",
        "external/nvapi",
        "external/stb",
    }

    links { 
        "glfw", 
        "opengl32",
        "nvapi64",
    }

    libdirs {
        "external/nvapi/amd64",
    }

    filter "system:windows"
        systemversion "latest"
        defines { "WIN32", "_WINDOWS" }

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

project "GpuDetails"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "GpuDetails/**.h",
        "GpuDetails/**.cpp"
    }

    includedirs {
        "GpuDetails",
        "external/nvapi"
    }

    links { 
        "nvapi64"
    }

    libdirs {
        "external/nvapi/amd64"
    }

    filter "system:windows"
        systemversion "latest"
        defines { 
            "WIN32", 
            "_WINDOWS",
            "GPU_DETAILS_EXPORTS"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
