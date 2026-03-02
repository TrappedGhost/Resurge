#include "rgpch.h"
#include "FrameBuffer.h"

#include"Resug/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLFrameBuffer.h"
namespace Resug
{
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSrecification& spec)
    {
		RG_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFrameBuffer>(spec);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
    }
}