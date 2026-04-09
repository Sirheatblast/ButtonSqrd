workspace "BtnSqrd"
	architecture "x64"
	startproject "ButtonEditor"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	outputdir ="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir ={}
	IncludeDir["GLAD"] ="ButtonSqrd/vendor/glad/include"
	IncludeDir["GLFW"] ="ButtonSqrd/vendor/GLFW/include"
	IncludeDir["entt"]="ButtonSqrd/vendor/entt/include"
	IncludeDir["glm"]="ButtonSqrd/vendor/glm/"
	IncludeDir["stb_image"]="ButtonSqrd/vendor/stb-master/"
	IncludeDir["assimp"]="ButtonSqrd/vendor/assimp/assimp"
	IncludeDir["ImGui"]="ButtonSqrd/vendor/imgui"
	IncludeDir["ImGuizmo"] = "ButtonSqrd/vendor/ImGuizmo"
	IncludeDir["ImNodes"] = "ButtonSqrd/vendor/imnodes"
	IncludeDir["yaml"] = "ButtonSqrd/vendor/yaml/include"
	IncludeDir["PhysX"] = "ButtonSqrd/vendor/PhysX"
	IncludeDir["OpenAL"] = "ButtonSqrd/vendor/OpenAL"
	IncludeDir["MiniAudio"]="ButtonSqrd/vendor/MiniAudio"

	include ("ButtonSqrd/vendor/glad")
	include ("ButtonSqrd/vendor/ImGui")
	include ("ButtonSqrd/vendor/ImGuizmo")
	include ("ButtonSqrd/vendor/ImNodes")
	include ("ButtonSqrd/vendor/yaml")
	
	project "ButtonSqrd"
		location "ButtonSqrd"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}

		includedirs {
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.assimp}/include",
			"%{IncludeDir.ImGui}/imgui",
			"%{IncludeDir.ImGui}/backends",
			"%{IncludeDir.ImGuizmo}",
			"%{IncludeDir.ImNodes}",
			"%{IncludeDir.yaml}",
			"%{IncludeDir.PhysX}/physx/include",
			"%{IncludeDir.OpenAL}/include",
			"%{IncludeDir.MiniAudio}",
			"ButtonSqrd/src"
		}

		links {
			"GLAD",
			"ImGui",
			"ImGuizmo",
			"ImNodes",
			"yaml",
		}

		libdirs {"%{IncludeDir.assimp}/lib/Release","ButtonSqrd/vendor/GLFW/bin","ButtonSqrd/vendor/OpenAL/bin" }
		links { "assimp-vc143-mt.dll","assimp-vc143-mt.lib","glfw3.dll","glfw3dll.lib","OpenAL32.dll","OpenAL32.lib" }

		links{
			"PhysX_static_64.lib",
			"PhysXCommon_static_64.lib",
			"PhysXFoundation_static_64.lib",
			"PhysXExtensions_static_64.lib",
			"PhysXPvdSDK_static_64.lib",
			"PhysXCooking_static_64.lib",
		}

		filter "system:windows"
			systemversion "latest"

		defines	{
			"BTN_PLATFORM_WINDOWS",
			"USE_GLFW",
			"GLFW_DLL",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE",
			"_CRT_SECURE_NO_WARNINGS",
			"PX_PHYSX_STATIC_LIB",
		}

		filter "configurations:Debug"
			defines "BTN_DEBUG"
			runtime "Debug"
			symbols "on"
			libdirs{"%{IncludeDir.PhysX}/bin/debug" }

		filter "configurations:Release"
			defines "BTN_REALEASE"
			defines "NDEBUG"
			runtime "Release"
			optimize "on"
			libdirs{"%{IncludeDir.PhysX}/bin/release" }

		filter "configurations:Dist"
			defines "BTN_DIST"
			defines "NDEBUG"
			runtime "Release"
			optimize "on"

	project "ButtonEditor"
		require "vstudio"

		premake.override(premake.vstudio.vc2010, "projectReferences", function(base, prj)
			local refs = premake.project.getdependencies(prj, 'linkOnly')
			if #refs > 0 then
			premake.push('<ItemGroup>')
			for _, ref in ipairs(refs) do
				local relpath = premake.vstudio.path(prj, premake.vstudio.projectfile(ref))
				premake.push('<ProjectReference Include=\"%s\">', relpath)
				premake.callArray(premake.vstudio.vc2010.elements.projectReferences, prj, ref)
				premake.vstudio.vc2010.element("UseLibraryDependencyInputs", nil, "true")
				premake.pop('</ProjectReference>')
			end
			premake.pop('</ItemGroup>')
			end
		end)

		location "ButtonEditor"
		kind "WindowedApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/**.rc",
			"%{prj.name}/resource.h",
		}
		
		defines{
			"GLFW_DLL",
			"YAML_CPP_STATIC_DEFINE",
			"PX_PHYSX_STATIC_LIB",
			"_CRT_SECURE_NO_WARNINGS",
		}

		includedirs	{
			"ButtonSqrd/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.assimp}/include",
			"%{IncludeDir.ImGui}/imgui",
			"%{IncludeDir.ImGui}/backends",
			"%{IncludeDir.ImGuizmo}",
			"%{IncludeDir.ImNodes}",
			"%{IncludeDir.yaml}",
			"%{IncludeDir.PhysX}/physx/include",
			"%{IncludeDir.OpenAL}/include",
			"%{IncludeDir.MiniAudio}",
			"ButtonSqrd/src"
		}

		libdirs {"%{IncludeDir.assimp}/lib/Release","ButtonSqrd/vendor/GLFW/bin","ButtonSqrd/vendor/OpenAL/bin" }
		links { "assimp-vc143-mt.dll","assimp-vc143-mt.lib","glfw3.dll","glfw3dll.lib","OpenAL32.dll","OpenAL32.lib" }

		links{
			"PhysX_static_64.lib",
			"PhysXCommon_static_64.lib",
			"PhysXFoundation_static_64.lib",
			"PhysXExtensions_static_64.lib",
			"PhysXPvdSDK_static_64.lib",
			"PhysXCooking_static_64.lib",
		}

		postbuildcommands {
			"{COPY} ../%{IncludeDir.assimp}/bin/Release/assimp-vc143-mt.dll ../bin/" .. outputdir .. "/%{prj.name}",
			"{COPY} ../ButtonSqrd/vendor/GLFW/bin/glfw3.dll ../bin/" .. outputdir .. "/%{prj.name}",
			"{COPY} ../ButtonSqrd/vendor/OpenAL/bin/OpenAL32.dll ../bin/" .. outputdir .. "/%{prj.name}",
			"{COPY} ../%{prj.name}/Assets ../bin/" .. outputdir .. "/%{prj.name}/Assets",
			"{COPY} ../%{prj.name}/EngineAssets ../bin/" .. outputdir .. "/%{prj.name}/EngineAssets",
			"{COPY} ../%{prj.name}/imgui.ini ../bin/" .. outputdir .. "/%{prj.name}",
    	}

		links
		{
			"ButtonSqrd"
		}

		filter "system:windows"
			systemversion "latest"
			linkoptions { "/ENTRY:mainCRTStartup" }
			defines
			{
				"BTN_PLATFORM_WINDOWS",
			}

		filter "configurations:Debug"
			defines "BTN_DEBUG"
			runtime "Debug"
			symbols "on"
			libdirs{"%{IncludeDir.PhysX}/bin/debug" }

		filter "configurations:Release"
			defines "BTN_REALEASE"
			defines "NDEBUG"
			runtime "Release"
			optimize "on"
			libdirs{"%{IncludeDir.PhysX}/bin/release" }

		filter "configurations:Dist"
			defines "BTN_DIST"
			runtime "Release"
			defines "NDEBUG"
			optimize "on"

	project "ButtonRuntime"
		require "vstudio"

		premake.override(premake.vstudio.vc2010, "projectReferences", function(base, prj)
			local refs = premake.project.getdependencies(prj, 'linkOnly')
			if #refs > 0 then
			premake.push('<ItemGroup>')
			for _, ref in ipairs(refs) do
				local relpath = premake.vstudio.path(prj, premake.vstudio.projectfile(ref))
				premake.push('<ProjectReference Include=\"%s\">', relpath)
				premake.callArray(premake.vstudio.vc2010.elements.projectReferences, prj, ref)
				premake.vstudio.vc2010.element("UseLibraryDependencyInputs", nil, "true")
				premake.pop('</ProjectReference>')
			end
			premake.pop('</ItemGroup>')
			end
		end)

		location "ButtonRuntime"
		kind "WindowedApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}
		
		defines{
			"GLFW_DLL",
			"YAML_CPP_STATIC_DEFINE",
			"PX_PHYSX_STATIC_LIB",
			"_CRT_SECURE_NO_WARNINGS",
		}

		includedirs	{
			"ButtonSqrd/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.assimp}/include",
			"%{IncludeDir.ImGui}/imgui",
			"%{IncludeDir.ImGui}/backends",
			"%{IncludeDir.ImGuizmo}",
			"%{IncludeDir.ImNodes}",
			"%{IncludeDir.yaml}",
			"%{IncludeDir.PhysX}/physx/include",
			"%{IncludeDir.OpenAL}/include",
			"%{IncludeDir.MiniAudio}",
			"ButtonSqrd/src"
		}

		libdirs {"%{IncludeDir.assimp}/lib/Release","ButtonSqrd/vendor/GLFW/bin","ButtonSqrd/vendor/OpenAL/bin" }
		links { "assimp-vc143-mt.dll","assimp-vc143-mt.lib","glfw3.dll","glfw3dll.lib","OpenAL32.dll","OpenAL32.lib" }

		links{
			"PhysX_static_64.lib",
			"PhysXCommon_static_64.lib",
			"PhysXFoundation_static_64.lib",
			"PhysXExtensions_static_64.lib",
			"PhysXPvdSDK_static_64.lib",
			"PhysXCooking_static_64.lib",
		}

		postbuildcommands {
			"{COPY} ../%{IncludeDir.assimp}/bin/Release/assimp-vc143-mt.dll ../bin/" .. outputdir .. "/%{prj.name}",
			"{COPY} ../ButtonSqrd/vendor/GLFW/bin/glfw3.dll ../bin/" .. outputdir .. "/%{prj.name}",
			"{COPY} ../ButtonSqrd/vendor/OpenAL/bin/OpenAL32.dll ../bin/" .. outputdir .. "/%{prj.name}",
			"{COPY} ../%{prj.name}/Assets ../bin/" .. outputdir .. "/%{prj.name}/Assets",
			"{COPY} ../%{prj.name}/EngineAssets ../bin/" .. outputdir .. "/%{prj.name}/EngineAssets",
			"{COPY} ../%{prj.name}/imgui.ini ../bin/" .. outputdir .. "/%{prj.name}",
    	}

		links
		{
			"ButtonSqrd"
		}

		filter "system:windows"
			systemversion "latest"
			linkoptions { "/ENTRY:mainCRTStartup" }
			
			defines
			{
				"BTN_PLATFORM_WINDOWS",
			}

		filter "configurations:Debug"
			defines "BTN_DEBUG"
			runtime "Debug"
			symbols "on"
			libdirs{"%{IncludeDir.PhysX}/bin/debug" }

		filter "configurations:Release"
			defines "BTN_REALEASE"
			defines "NDEBUG"
			runtime "Release"
			optimize "on"
			libdirs{"%{IncludeDir.PhysX}/bin/release" }

		filter "configurations:Dist"
			defines "BTN_DIST"
			runtime "Release"
			defines "NDEBUG"
			optimize "on"