workspace "Renaissance"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}
	
	startproject "Sandbox"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	IncludeDir = {}
	IncludeDir["spdlog"] = "Renaissance/externals/spdlog/include"
	IncludeDir["GLFW"] = "Renaissance/externals/GLFW/include"
	IncludeDir["Glad"] = "Renaissance/externals/Glad/include"
	IncludeDir["ImGui"] = "Renaissance/externals/ImGui/"
	IncludeDir["glm"] = "Renaissance/externals/glm"
	IncludeDir["stb_image"] = "Renaissance/externals/stb_image"

	include "Renaissance/externals/GLFW"
	include "Renaissance/externals/Glad"
	include "Renaissance/externals/ImGui"

	project "Renaissance"
		location "Renaissance"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		
		sourcedir = "%{prj.name}/src"
		pchheader "RenaissancePCH.h"
		pchsource(sourcedir.."/RenaissancePCH.cpp")

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

		files 
		{
			"%{prj.name}/include/**.h",
			"%{prj.name}/src/RenaissancePCH.cpp",
			"%{prj.name}/src/Renaissance/Core/**.cpp",
			"%{prj.name}/src/Renaissance/Editor/**.cpp",
			"%{prj.name}/src/Renaissance/Graphics/**.cpp",
			"%{prj.name}/src/Renaissance/UserInterface/**.cpp",
			"%{prj.name}/externals/stb_image/stb_image.cpp"
		}

		includedirs
		{
			"%{prj.name}/include",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb_image}"
		}
		
		links
		{
			"GLFW",
			"Glad",
			"ImGui"
		}

		defines
		{			
			"_CRT_SECURE_NO_WARNINGS"
		}

		filter "system:windows"
			systemversion "latest"

			links 
			{
				"opengl32.lib"
			}

			files 
			{
				"%{prj.name}/src/Renaissance/Platform/Windows/**.cpp",
				"%{prj.name}/src/Renaissance/Platform/OpenGL/**.cpp"
			}

		filter "system:macosx"
			files 
			{
				"%{prj.name}/src/Renaissance/Platform/OSX/**.cpp",
				"%{prj.name}/src/Renaissance/Platform/OpenGL/**.cpp"
			}

		filter "configurations:Debug"
			defines "REN_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "REN_RELEASE"
			optimize "On"

		filter "configurations:Shipping"
			defines "REN_SHIPPING"
			optimize "On"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		sourcedir = "%{prj.name}/src"
		pchheader "SandboxPCH.h"
		pchsource(sourcedir.."/SandboxPCH.cpp")

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

		files 
		{
			"%{prj.name}/include/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/include",
			"Renaissance/include",
			"Renaissance/externals/spdlog/include",
			"%{IncludeDir.glm}",
			"%{IncludeDir.ImGui}"
		}

		links
		{
			"Renaissance"
		}

		filter "system:windows"
			systemversion "latest"

		filter "configurations:Debug"
			defines "REN_DEBUG"
			symbols "on"

		filter "configurations:Release"
			defines "REN_RELEASE"
			optimize "on"

		filter "configurations:Shipping"
			defines "REN_SHIPPING"
			optimize "on"