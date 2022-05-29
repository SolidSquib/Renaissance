project "Renaissance"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	sourcedir = "src"
	pchheader "RenaissancePCH.h"
	pchsource(sourcedir.."/RenaissancePCH.cpp")

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"include/**.h",
		"src/RenaissancePCH.cpp",
		"src/Renaissance/Core/**.cpp",		
		"src/Renaissance/Graphics/**.cpp",
		"src/Renaissance/UserInterface/**.cpp",
		"src/Renaissance/Scene/**.cpp",
		"externals/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"include",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
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
			"src/Renaissance/Platform/Windows/**.cpp",
			"src/Renaissance/Platform/OpenGL/**.cpp"
		}

	filter "system:macosx"
		files 
		{
			"src/Renaissance/Platform/OSX/**.cpp",
			"src/Renaissance/Platform/OpenGL/**.cpp"
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