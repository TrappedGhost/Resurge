#pragma once

#include"Resug/Core/Core.h"
#include"Resug/Core/Log.h"

#include<glm/glm.hpp>

namespace Resug
{
	class Shader
	{
	public:
		~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string vertexSrc, const std::string fragmentSrc);

	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string vertexSrc, const std::string fragmentSrc);
		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}

