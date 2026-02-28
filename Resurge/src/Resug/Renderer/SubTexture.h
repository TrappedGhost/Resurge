#pragma once


#include<glm/glm.hpp>
#include"Texture.h"

namespace Resug
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		Ref<SubTexture2D> CreateSubTex2D(const Ref<Texture2D>& texture);
		Ref<Texture2D> GetTexture() { return m_Texture; }
		glm::vec2* GetTexCoords() { return m_TexCoords; }
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};

}