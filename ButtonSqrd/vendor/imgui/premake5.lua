project "ImGui"
    kind "StaticLib"
    language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
		"imgui/**.h",
		"imgui/**.cpp",

		"backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_glfw.h",
        "backends/imgui_impl_opengl3.cpp",
        "backends/imgui_impl_glfw.cpp"
    }

    includedirs { "imgui", "backends" }

	links { "glfw3dll.lib" }
	includedirs { "../GLFW/include" }
    libdirs { "../GLFW/bin" }

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

