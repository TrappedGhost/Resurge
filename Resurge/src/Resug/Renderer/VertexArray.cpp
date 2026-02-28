#include "rgpch.h"
#include "VertexArray.h"
#include"Resug/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace Resug
{
	Ref<VertexArray> VertexArray::Create()
	{
		RG_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}