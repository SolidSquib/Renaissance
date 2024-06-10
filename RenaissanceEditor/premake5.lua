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
		"externals/ImGuizmo/ImGuizmo.h",
		"externals/ImGuizmo/ImGuizmo.cpp"
	}
	
	includedirs
	{
		"include",
		"%{wks.location}/Renaissance/include",
		"%{IncludeDir.cereal}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.IconFonts}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.spdlog}"
	}
	
	links
	{
		"Renaissance"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}
	
	filter "files:%{wks.location}/Renaissance/externals/spdlog/include/**.h"
		warnings "Off"

	filter "files:externals/ImGuizmo/**.cpp"
		flags { "NoPCH" }

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