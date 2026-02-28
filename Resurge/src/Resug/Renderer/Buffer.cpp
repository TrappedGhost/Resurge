#include"rgpch.h"


#include"Resug/Renderer/Buffer.h"

#include"Resug/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Resug
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		RG_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		RG_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
			case RendererAPI::API::OpenGL:  return  CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		RG_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
