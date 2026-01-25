#include "rgpch.h"
#include "RendererCommand.h"

#include"Platform/OpenGL/OpenGLRendererAPI.h"

namespace Resug
{
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();
}