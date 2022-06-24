-- premake5.lua
workspace "DataPak"
    architecture "x64"
    configurations {"Debug", "Release"}

project "datapak"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/"
    objdir "bin/obj/"

    files {"src/datapak.cpp"}
    includedirs {"external/"}
    libdirs {"external/"}
    links {"snappy"}
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "test"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/"
    objdir "bin/obj/"

    files {"test/main.cpp"}
    includedirs {"src/"}
    links {"datapak"}
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"