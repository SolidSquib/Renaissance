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
	IncludeDir["GLFW"] = "Renaissance/externals/GLFW/include"

	include "Renaissance/externals/GLFW"

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
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/include",
			"%{prj.name}/externals/spdlog/include",
			"%{IncludeDir.GLFW}"
		}
		
		links
		{
			"GLFW"
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
			"Renaissance/externals/spdlog/include"
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