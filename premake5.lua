IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Renaissance/externals/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Renaissance/externals/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Renaissance/externals/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Renaissance/externals/ImGui/"
IncludeDir["glm"] = "%{wks.location}/Renaissance/externals/glm"
IncludeDir["stb_image"] = "%{wks.location}/Renaissance/externals/stb_image"
IncludeDir["entt"] = "%{wks.location}/Renaissance/externals/entt/include"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Renaissance"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	group("Engine")
		include "Renaissance"
		include "Sandbox"
		include "RenaissanceEditor"

	group("Externals")
		include "Renaissance/externals/GLFW"
		include "Renaissance/externals/Glad"
		include "Renaissance/externals/ImGui"