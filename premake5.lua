workspace "Renaissance"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	project "Renaissance"
		location "Renaissance"
		kind "SharedLib"
		language "C++"

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
			"%{prj.name}/externals/spdlog/include"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"REN_PLATFORM_WINDOWS",
				"REN_BUILD_DLL"
			}

			postbuildcommands
			{
				("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"REN_PLATFORM_WINDOWS"
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