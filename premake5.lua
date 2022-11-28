IncludeDir = {}
IncludeDir["cereal"] = "%{wks.location}/Renaissance/externals/cereal/include"
IncludeDir["entt"] = "%{wks.location}/Renaissance/externals/entt/include"
IncludeDir["Glad"] = "%{wks.location}/Renaissance/externals/Glad/include"
IncludeDir["GLFW"] = "%{wks.location}/Renaissance/externals/GLFW/include"
IncludeDir["glm"] = "%{wks.location}/Renaissance/externals/glm"
IncludeDir["IconFonts"] = "%{wks.location}/Renaissance/externals/IconFontCppHeaders"
IncludeDir["ImGui"] = "%{wks.location}/Renaissance/externals/ImGui/"
IncludeDir["ImGuizmo"] = "%{wks.location}/RenaissanceEditor/externals/ImGuizmo"
IncludeDir["spdlog"] = "%{wks.location}/Renaissance/externals/spdlog/include"
IncludeDir["stb_image"] = "%{wks.location}/Renaissance/externals/stb_image"
IncludeDir["yamlcpp"] = "%{wks.location}/Renaissance/externals/yaml-cpp/include"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Renaissance"
	architecture "x64"
	startproject "Faireground"

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
		include "RenaissanceEditor"

	group("Externals")
		include "Renaissance/externals/GLFW"
		include "Renaissance/externals/Glad"
		include "Renaissance/externals/ImGui"
		include "Renaissance/externals/yaml-cpp"
		include "Renaissance/externals/cereal"