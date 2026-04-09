project "yaml"
    kind "StaticLib"
    language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    defines{
        "YAML_CPP_STATIC_DEFINE"
    }

    files {
        "src/**.h",
        "src/**.cpp",

		"include/**.h",
    }

    includedirs { "include","src" }

filter "configurations:Debug"
    defines "BTN_DEBUG"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines "BTN_REALEASE"
    runtime "Release"
    optimize "on"

filter "configurations:Dist"
    defines "BTN_DIST"
    runtime "Release"
    optimize "on"

	filter "system:windows"
		systemversion "latest"

