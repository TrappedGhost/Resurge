#include "rgpch.h"
#include "Shader.h"

#include"Resug/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace Resug
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
		RG_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, filepath);
		} 
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	
	Ref<Shader> Shader::Create(const std::string& name, const std::string vertexSrc, const std::string fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RG_CORE_ASSERT(false, "RendererAPI: None is not sipport!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		RG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		RG_CORE_INFO("{0}", name);
		RG_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader Already exist!");
		m_Shaders[name] = shader;

		//m_Shaders["Texture"] = nullptr;


	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		RG_CORE_INFO("Load");
		auto shader = Shader::Create(name, filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string vertexSrc, const std::string fragmentSrc)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{

		RG_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end(); 
	}
}