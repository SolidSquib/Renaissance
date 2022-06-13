#include "FairegroundPCH.h"

#include "Renaissance/Core/EntryPoint.h"
#include "Faireground/EditorLayer.h"

namespace Renaissance
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication(const std::string& name, ApplicationCommandLineArgs args)
			: Application(name, args)
		{
			mEditor = CreateNewLayer<EditorLayer>();
		}

		virtual ~EditorApplication()
		{
		}

	private:
		WeakPtr<EditorLayer> mEditor;
	};

	Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args)
	{
		return new EditorApplication("Faireground", args);
	}
}
