workspace "ScriptBuild"
   configurations { "Release", "Debug" }
   platforms { "x64" }
   location "EngineAssets/Scripts"

   IncludeDir = {}
   IncludeDir["spdlog"] = "../ButtonSqrd/vendor/spdlog/include"
   IncludeDir["GLAD"] = "../ButtonSqrd/vendor/glad/include"
   IncludeDir["GLFW"] = "../ButtonSqrd/vendor/GLFW/include"
   IncludeDir["entt"] = "../ButtonSqrd/vendor/entt/include"
   IncludeDir["glm"] = "../ButtonSqrd/vendor/glm/"
   IncludeDir["stb_image"] = "../ButtonSqrd/vendor/stb-master/"
   IncludeDir["assimp"] = "../ButtonSqrd/vendor/assimp/assimp"
   IncludeDir["ImGui"] = "../ButtonSqrd/vendor/imgui"
   IncludeDir["ImGuizmo"] = "../ButtonSqrd/vendor/ImGuizmo"
   IncludeDir["yaml"] = "../ButtonSqrd/vendor/yaml/include"
   IncludeDir["PhysX"] = "../ButtonSqrd/vendor/PhysX"
   IncludeDir["OpenAL"] = "../ButtonSqrd/vendor/OpenAL"
	IncludeDir["MiniAudio"]="../ButtonSqrd/vendor/MiniAudio"

project "BtnScripts"
   kind "SharedLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"
   targetdir "./EngineAssets/Scripts/bin"
   objdir "./EngineAssets/Scripts/bin-int"

   files {
      "./EngineAssets/Scripts/include/ScriptRegistry.h",
      "./EngineAssets/Scripts/include/ScriptRegistry.cpp",

      "./Assets/Scripts/**.h",
      "./Assets/Scripts/**.cpp"
   }

   includedirs {
      "EngineAssets/Scripts/include",
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLAD}",
      "%{IncludeDir.glm}",
      "%{IncludeDir.entt}",
      "%{IncludeDir.stb_image}",
      "%{IncludeDir.assimp}/include",
      "%{IncludeDir.ImGui}/imgui",
      "%{IncludeDir.ImGui}/backends",
      "%{IncludeDir.ImGuizmo}",
      "%{IncludeDir.yaml}",
      "%{IncludeDir.PhysX}/physx/include",
      "%{IncludeDir.OpenAL}/include",
		"%{IncludeDir.MiniAudio}",
      "../ButtonSqrd/src"
   }

   defines{
      "GLFW_DLL",
   }

   links {
      "ButtonSqrd.lib",
      "glfw3.dll",
      "GLAD.lib",
      "ImGui.lib",
      "yaml.lib",
   }

   filter "platforms:x64"
      architecture "x64"

   filter "configurations:Release"
      runtime "Release"
      optimize "On"
      symbols "Off"
      defines { 
		   "NDEBUG",
	   }
      libdirs {
         "../bin/Release-windows-x86_64/ButtonSqrd",
         "../ButtonSqrd",
         "../ButtonSqrd/vendor/GLFW/bin/Release-windows-x86_64/GLFW",
         "../ButtonSqrd/vendor/glad/bin/Release-windows-x86_64/GLAD",
         "../ButtonSqrd/vendor/imgui/bin/Release-windows-x86_64/ImGui",
         "../ButtonSqrd/vendor/yaml/bin/Release-windows-x86_64/yaml",
      }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "On"
      defines {
         "BTN_DEBUG",
      }
      libdirs {
         "../bin/Debug-windows-x86_64/ButtonSqrd",
         "../ButtonSqrd",
         "../ButtonSqrd/vendor/GLFW/bin/Debug-windows-x86_64/GLFW/bin",
         "../ButtonSqrd/vendor/glad/bin/Debug-windows-x86_64/GLAD",
         "../ButtonSqrd/vendor/imgui/bin/Debug-windows-x86_64/ImGui",
         "../ButtonSqrd/vendor/yaml/bin/Debug-windows-x86_64/yaml",
      }
   filter "system:windows"
      systemversion "latest"
      defines { 
         "BTN_PLATFORM_WINDOWS",
			"USE_GLFW",
			"GLFW_DLL",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE",
			"_CRT_SECURE_NO_WARNINGS",
			"STB_IMAGE_IMPLEMENTATION",
			"PX_PHYSX_STATIC_LIB",
      }