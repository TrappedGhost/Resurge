#include"rgpch.h"


#include"Resug/Renderer/Buffer.h"

#include"Resug/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Resug
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, count);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
