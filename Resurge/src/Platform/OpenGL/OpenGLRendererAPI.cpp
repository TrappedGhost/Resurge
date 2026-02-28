#include "rgpch.h"
#include "OpenGLRendererAPI.h"

#include<glad/glad.h>

void Resug::OpenGLRendererAPI::Init()
{
	RG_PROFILE_FUNCTION();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void Resug::OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void Resug::OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	RG_PROFILE_FUNCTION();

	glClearColor(color.r, color.g, color.b, color.a);
}

void Resug::OpenGLRendererAPI::Clear()
{
	RG_PROFILE_FUNCTION();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Resug::OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const uint32_t count)
{
	RG_PROFILE_FUNCTION();

	uint32_t Count = count ? count : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}
