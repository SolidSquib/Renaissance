project "[PROJECT_NAME]"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	sourcedir = "src"
	pchheader "[PROJECT_NAME]PCH.h"
	pchsource(sourcedir.."/[PROJECT_NAME]PCH.cpp")
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")
	
	files 
	{
		"include/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"include",
		"%{EnginePath}/include",
		"%{EnginePath}/externals/cereal/include",
		"%{EnginePath}/externals/entt/include",
		"%{EnginePath}/externals/glm",
		"%{EnginePath}/externals/IconFonts}",
		"%{EnginePath}/externals/ImGui",
		"%{EnginePath}/externals/spdlog/include"
	}
	
	links
	{
		"Renaissance"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}
	
	filter "files:%{EnginePath}/externals/spdlog/include/**.h"
		warnings "Off"

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