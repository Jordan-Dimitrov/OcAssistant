workspace "OcAssitant"

    startproject "OcAssitant"
    
    configurations 
    {
        "Debug",
        "Release"
    }

    flags 
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "OcAssistant/vendor/glfw"
include "OcAssistant/vendor/glad"
include "OcAssistant"
