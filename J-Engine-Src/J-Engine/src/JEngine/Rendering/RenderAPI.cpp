#include <JEngine/Rendering/RenderAPI.h>

namespace JEngine {
	RenderAPI::API RenderAPI::_api = RenderAPI::API::OpenGL;

	RenderAPI* RenderAPI::create()
	{
		switch (_api)
		{
			case RenderAPI::API::None:    return nullptr;
			case RenderAPI::API::OpenGL:  return nullptr;
		}


		return nullptr;
	}
}