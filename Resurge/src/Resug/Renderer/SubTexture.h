#pragma once


#include<glm/glm.hpp>
#include"Texture.h"

namespace Resug
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		Ref<Texture2D> GetTexture() { return m_Texture; }
		glm::vec2* GetTexCoords() { return m_TexCoords; }
		
		static Ref<SubTexture2D> CreateSubTex2D(const Ref<Texture2D>& texture, glm::vec2 texCount,glm::vec2 length, glm::vec2 texSize);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};

}