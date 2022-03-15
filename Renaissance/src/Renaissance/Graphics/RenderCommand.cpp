#include "RenaissancePCH.h"
#include "Renaissance/Graphics/RenderCommands.h"

namespace Renaissance::Graphics
{
	UniquePtr<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::Create();
}