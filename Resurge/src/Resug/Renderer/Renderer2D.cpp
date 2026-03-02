#include "rgpch.h"
#include"RendererCommand.h"
#include "Renderer2D.h"
namespace Resug
{
	struct QuadVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TexZoomLevel;
	};

	struct Renderer2DData
	{
		uint32_t MaxQuads = 200;
		uint32_t MaxVertices = 4 * MaxQuads;
		uint32_t MaxIndices = 6 * MaxQuads;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t IndicesCount;
		uint32_t TextureSlotsIndex;

		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;

		glm::vec4 QuadVertexPosition[4];
		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
	
		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		RG_PROFILE_FUNCTION();

		s_Data.VertexArray = Resug::VertexArray::Create();


		s_Data.VertexBuffer = Resug::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.VertexBuffer->SetLayout({
			{Resug::ShaderDataType::Float4, "a_Position"},
			{Resug::ShaderDataType::Float4, "a_Color"},
			{Resug::ShaderDataType::Float2, "a_TexCoord"},
			{Resug::ShaderDataType::Float , "a_TexIndex"},
			{Resug::ShaderDataType::Float , "a_TexZoomLevel"}
			});

		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (int i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		s_Data.IndexBuffer = Resug::IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.VertexArray->SetIndexBuffer(s_Data.IndexBuffer);


		int32_t shaderBindTexIndex[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			shaderBindTexIndex[i] = i;
		}
		
		s_Data.TextureShader =  Shader::Create("FlatColor", "Assert/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadIntArray("u_Textures", shaderBindTexIndex, s_Data.MaxTextureSlots);


		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPosition[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

	}
	void Renderer2D::Shutdown()
	{
		RG_PROFILE_FUNCTION();
	}
	void Renderer2D::WindowResize(uint32_t width, uint32_t height)
	{
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		RG_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.IndicesCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotsIndex = 1;
	}
	void Renderer2D::EndScene()
	{
		uint32_t DataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;

		s_Data.VertexArray->SetData(s_Data.QuadVertexBufferBase, DataSize);

		Flush();
	}
	void Renderer2D::Flush()
	{
		for (int i = 0; i < s_Data.TextureSlotsIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RendererCommand::DrawIndexed(s_Data.VertexArray, s_Data.IndicesCount);
		
		s_Data.Stats.DrawCalls++;
	}
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.IndicesCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotsIndex = 1;
	}
	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		Renderer2D::DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
	{
		RG_PROFILE_FUNCTION();
		
		if (s_Data.Stats.QuadCount >= s_Data.MaxQuads)
		{
			FlushAndReset();
		}

		float texIndex = 0.0f;
		float texZoomLevel = 1.0f;
		glm::vec2 texCoords[4] = {
			{0.0f,0.0f},
			{0.0f,0.0f},
			{0.0f,0.0f},
			{0.0f,0.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = {0.0f,0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;
\
		s_Data.IndicesCount+=6;

		s_Data.Stats.QuadCount++;

	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel, glm::vec4 tintColor)
	{
		Renderer2D::DrawQuad({ position.x, position.y, 0.0f }, size, texture, texZoomLevel, tintColor);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel, glm::vec4 tintColor)
	{
		RG_PROFILE_FUNCTION();
		if (s_Data.Stats.QuadCount >= s_Data.MaxQuads)
		{
			FlushAndReset();
		}

		glm::vec4 color = tintColor;
		glm::vec2 texCoords[4] = { 
			{0.0f,0.0f},
			{1.0f,0.0f},
			{1.0f,1.0f},
			{0.0f,1.0f } };

		float texIndex = 0.0f;
		for (int i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = i;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = s_Data.TextureSlotsIndex; 
			s_Data.TextureSlots[s_Data.TextureSlotsIndex] = texture;
			s_Data.TextureSlotsIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCount++;

	}
	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float texZoomLevel, glm::vec4 tintColor)
	{
		Renderer2D::DrawQuad({ position.x, position.y, 0.1f }, size, subTexture, texZoomLevel, tintColor);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float texZoomLevel, glm::vec4 tintColor)
	{
		RG_PROFILE_FUNCTION();
		if (s_Data.Stats.QuadCount >= s_Data.MaxQuads)
		{
			FlushAndReset();
		}

		glm::vec4 color = tintColor;
		glm::vec2* texCoords = subTexture->GetTexCoords();
		Ref<Texture2D> texture = subTexture->GetTexture();

		float texIndex = 0.0f;
		for (int i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = i;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = s_Data.TextureSlotsIndex;
			s_Data.TextureSlots[s_Data.TextureSlotsIndex] = texture;
			s_Data.TextureSlotsIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCount++;

	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color)
	{
		Renderer2D::DrawRotatedQuad({ position.x, position.y, 0.0f }, size,rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color)
	{
		RG_PROFILE_FUNCTION();
		if (s_Data.Stats.QuadCount >= s_Data.MaxQuads)
		{
			FlushAndReset();
		}
		float texIndex = 0.0f;
		float texZoomLevel = 1.0f;
		glm::vec2 texCoords[4] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{1.0f,1.0f},
			{0.0f,1.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.IndicesCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float texZoomLevel,  glm::vec4 tintColor )
	{
		Renderer2D::DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation,texture, texZoomLevel, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float texZoomLevel, glm::vec4 tintColor)
	{
		RG_PROFILE_FUNCTION();
		if (s_Data.Stats.QuadCount >= s_Data.MaxQuads)
		{
			FlushAndReset();
		}
		glm::vec4 color = tintColor;
		glm::vec2 texCoords[4] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{1.0f,1.0f},
			{0.0f,1.0f } };

		float texIndex = 0.0f;
		for (int i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = i;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = s_Data.TextureSlotsIndex;
			s_Data.TextureSlots[s_Data.TextureSlotsIndex] = texture;
			s_Data.TextureSlotsIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.IndicesCount += 6;
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<SubTexture2D> subTexture, float texZoomLevel, glm::vec4 tintColor)
	{
		Renderer2D::DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, texZoomLevel, tintColor);

	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<SubTexture2D> subTexture, float texZoomLevel, glm::vec4 tintColor)
	{
		RG_PROFILE_FUNCTION();
		if (s_Data.Stats.QuadCount >= s_Data.MaxQuads)
		{
			FlushAndReset();
		}

		glm::vec4 color = tintColor;
		glm::vec2* texCoords = subTexture->GetTexCoords();
		Ref<Texture2D> texture = subTexture->GetTexture();

		float texIndex = 0.0f;
		for (int i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = i;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = s_Data.TextureSlotsIndex;
			s_Data.TextureSlots[s_Data.TextureSlotsIndex] = texture;
			s_Data.TextureSlotsIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}