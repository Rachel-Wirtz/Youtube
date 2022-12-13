workspace "Youtube"
    cppdialect      "C++20"
    cdialect        "C17"
    characterset    "MBCS"
    targetdir       "%{wks.location}/output/bin/%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}/"
    objdir          "%{wks.location}/output/obj/%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}/"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    platforms {
        "x86",
        "x64"
    }

    files {
        "%{prj.location}/**.h",
        "%{prj.location}/**.hpp",
        "%{prj.location}/**.c",
        "%{prj.location}/**.cpp"
    }

    filter "platforms:x86"
        defines         "RW_ARCH_X86"
        architecture    "x86"

    filter "platforms:x64"
        defines         "RW_ARCH_X64"
        architecture    "x64"

    filter "configurations:Debug"
        defines         "RW_BUILD_DEBUG"
        symbols         "On"
        optimize        "Off"

    filter "configurations:Release"
        defines         "RW_BUILD_RELEASE"
        symbols         "On"
        optimize        "On"

    filter "configurations:Dist"
        defines         "RW_BUILD_DIST"
        symbols         "Off"
        optimize        "On"

    filter "system:windows"
        defines         "RW_PLATFORM_WINDOWS"

    filter "system:linux"
        defines         "RW_PLATFORM_LINUX"

    project "Collection"
        kind            "SharedItems"
        location        "Collection"

    project "Sandbox"
        kind            "ConsoleApp"
        location        "Sandbox"
        language        "C++"
        staticruntime   "Off"
        links           "Collection"