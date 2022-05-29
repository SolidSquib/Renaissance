project "Faireground"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	sourcedir = "src"
	pchheader "FairegroundPCH.h"
	pchsource(sourcedir.."/FairegroundPCH.cpp")
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")
	
	files 
	{
		"include/**.h",
		"src/**.cpp",
		"src/Faireground/Windows/**.cpp",
	}
	
	includedirs
	{
		"include",
		"%{wks.location}/Renaissance/include",
		"%{wks.location}/Renaissance/externals/spdlog/include",
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