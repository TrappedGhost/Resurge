#pragma once

#include"Resug/Core.h"
#include"Resug/Log.h"

#include<glm/glm.hpp>

namespace Resug
{
	class Shader
	{
	public:
		Shader(const std::string vertexSrc, const std::string fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string name,const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};
}

