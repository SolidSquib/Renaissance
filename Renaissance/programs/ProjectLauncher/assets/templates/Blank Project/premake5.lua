EnginePath = os.getenv("RenaissanceEngine") .. "/Renaissance"
EditorPath = os.getenv("RenaissanceEngine") .. "/RenaissanceEditor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "[PROJECT_NAME]"
	architecture "x64"
	startproject "[PROJECT_NAME]"

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

	group("Project")
		include "[PROJECT_NAME]"
		include "[PROJECT_NAME]Editor"

	group("Engine")
		include "%{EnginePath}"

	group("Externals")
		include "%{EnginePath}/externals/GLFW"
		include "%{EnginePath}/externals/Glad"
		include "%{EnginePath}/externals/ImGui"
		include "%{EnginePath}/externals/yaml-cpp"
		include "%{EnginePath}/externals/cereal"