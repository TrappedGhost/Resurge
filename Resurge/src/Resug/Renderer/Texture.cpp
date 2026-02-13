#include "rgpch.h"
#include "Texture.h"
#include"Resug/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLTexture.h"

namespace Resug
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}