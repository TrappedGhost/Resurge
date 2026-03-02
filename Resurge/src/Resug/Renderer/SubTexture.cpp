#include "rgpch.h"
#include "SubTexture.h"


namespace Resug
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		:m_Texture(texture)
	{
		m_TexCoords[0] = glm::vec2(min.x, min.y);
		m_TexCoords[1] = glm::vec2(max.x, min.y);
		m_TexCoords[2] = glm::vec2(max.x, max.y);
		m_TexCoords[3] = glm::vec2(min.x, max.y);
	}

	Ref<SubTexture2D> SubTexture2D::CreateSubTex2D(const Ref<Texture2D>& texture, glm::vec2 texCount,glm::vec2 length, glm::vec2 texSize)
	{
		glm::vec2 min = glm::vec2(
			texCount.x / texSize.x,
			texCount.y / texSize.y
		);

		glm::vec2 max = glm::vec2(
			(texCount.x + length.x) / texSize.x,
			(texCount.y + length.y) / texSize.y
		);

		return CreateRef<SubTexture2D>(texture, min, max);
	}
}