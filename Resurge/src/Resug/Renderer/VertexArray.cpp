#include "rgpch.h"
#include "VertexArray.h"
#include"Resug/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace Resug
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}