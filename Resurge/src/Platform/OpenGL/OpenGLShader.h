#pragma once

#include"Resug/Renderer/Shader.h"

typedef unsigned GLenum;

namespace Resug
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader() {}

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void UploadInt(const std::string name, const uint value) override;
		virtual void UploadMat3(const std::string name, const glm::mat3& value) override;
		virtual void UploadMat4(const std::string name, const glm::mat4& value) override;
		virtual void UploadFloat2(const std::string name, const glm::vec2& value) override;
		virtual void UploadFloat3(const std::string name, const glm::vec3& value) override;
		virtual void UploadFloat4(const std::string name, const glm::vec4& value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string name, const int value);

		void UploadUniformFloat(const std::string name, const float value);
		void UploadUniformFloat2(const std::string name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string name, const glm::vec4& value);

		void UploadUniformMat3(const std::string name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
		
	private:

		uint32_t m_RendererID;

		std::string m_Name;
	};
}


