#include "rgpch.h"
#include"RendererCommand.h"
#include "Renderer2D.h"
namespace Resug
{
	struct CircleVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TexZoomLevel;

	};
	struct QuadVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TexZoomLevel;
	};
	struct TriangleVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TexZoomLevel;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxTextureSlots = 32;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;

		Renderer2D::Statistics Stats;
		////////////////////////////////////////////Quad///////////////////////////////////////////////////////
		uint32_t MaxQuads = 200;
		uint32_t MaxVertices = 4 * MaxQuads;
		uint32_t MaxIndices = 6 * MaxQuads;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;

		uint32_t IndicesCount;
		uint32_t TextureSlotsIndex;

		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;

		glm::vec4 QuadVertexPosition[4];

		////////////////////////////////////////////Circle///////////////////////////////////////////////////////
		uint32_t MaxCircles = 200;
		uint32_t SegmentsPerCircle = 20;
		uint32_t MaxCircleVertices = MaxCircles * (SegmentsPerCircle + 1);
		uint32_t MaxCircleIndices = MaxCircles * SegmentsPerCircle * 3;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<IndexBuffer> CircleIndexBuffer;
	
		glm::vec4 CircleVertexPosition[21];
		uint32_t CircleIndicesCount;
	
		CircleVertex* CircleVertexBufferBase;
		CircleVertex* CircleVertexBufferPtr;

		////////////////////////////////////////////Triangle///////////////////////////////////////////////////////
		uint32_t MaxTriangles = 200;
		uint32_t MaxTriangleVertices = 3 * MaxTriangles;
		uint32_t MaxTriangleIndices = 3 * MaxTriangles;

		Ref<VertexArray> TriangleVertexArray;
		Ref<VertexBuffer> TriangleVertexBuffer;
		Ref<IndexBuffer> TriangleIndexBuffer;

		glm::vec4 TriangleVertexPosition[3];
		uint32_t TriangleIndicesCount;

		TriangleVertex* TriangleVertexBufferBase;
		TriangleVertex* TriangleVertexBufferPtr;


	};

	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		RG_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = Resug::VertexArray::Create();


		s_Data.QuadVertexBuffer = Resug::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout({
			{Resug::ShaderDataType::Float4, "a_Position"},
			{Resug::ShaderDataType::Float4, "a_Color"},
			{Resug::ShaderDataType::Float2, "a_TexCoord"},
			{Resug::ShaderDataType::Float , "a_TexIndex"},
			{Resug::ShaderDataType::Float , "a_TexZoomLevel"}
			});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

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
		s_Data.QuadIndexBuffer = Resug::IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);

		s_Data.QuadVertexPosition[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

		Renderer2D::InitTriangle();


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


	}

	//////////////////////////////////////////////////InitCircle/////////////////////////////////////////////////
	void Renderer2D::InitCircle()
	{
		//TODO: 完成圆的初始化
		s_Data.CircleVertexArray = VertexArray::Create();
		s_Data.CircleVertexBuffer = Resug::VertexBuffer::Create(s_Data.MaxCircleVertices * sizeof(CircleVertex));

		s_Data.CircleVertexBuffer->SetLayout({
			{Resug::ShaderDataType::Float4, "a_Position"},
			{Resug::ShaderDataType::Float4, "a_Color"},
			{Resug::ShaderDataType::Float2, "a_TexCoord"},
			{Resug::ShaderDataType::Float , "a_TexIndex"},
			{Resug::ShaderDataType::Float , "a_TexZoomLevel"}
			});

		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);

	}
	//////////////////////////////////////////////////InitTriangle/////////////////////////////////////////////////
	
	void Renderer2D::InitTriangle()
	{
		RG_PROFILE_FUNCTION();

		s_Data.TriangleVertexArray = Resug::VertexArray::Create();

		s_Data.TriangleVertexBuffer = Resug::VertexBuffer::Create(s_Data.MaxTriangleVertices * sizeof(TriangleVertex));

		s_Data.TriangleVertexBuffer->SetLayout({
			{Resug::ShaderDataType::Float4, "a_Position"},
			{Resug::ShaderDataType::Float4, "a_Color"},
			{Resug::ShaderDataType::Float2, "a_TexCoord"},
			{Resug::ShaderDataType::Float , "a_TexIndex"},
			{Resug::ShaderDataType::Float , "a_TexZoomLevel"}
			});

		s_Data.TriangleVertexArray->AddVertexBuffer(s_Data.TriangleVertexBuffer);

		s_Data.TriangleVertexBufferBase = new TriangleVertex[s_Data.MaxTriangleVertices];

		uint32_t* triangleIndices = new uint32_t[s_Data.MaxTriangleIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxTriangleIndices; i += 3)
		{
			triangleIndices[i + 0] = offset + 0;
			triangleIndices[i + 1] = offset + 1;
			triangleIndices[i + 2] = offset + 2;

			offset += 3;
		}

		s_Data.TriangleIndexBuffer = Resug::IndexBuffer::Create(triangleIndices, s_Data.MaxTriangleIndices);
		s_Data.TriangleVertexArray->SetIndexBuffer(s_Data.TriangleIndexBuffer);

		delete[] triangleIndices;

		const float height = 0.866f;  // sqrt(3)/2

		s_Data.TriangleVertexPosition[0] = glm::vec4(0.0f, -height / 2.0f, 0.0f, 1.0f);     // 左下
		s_Data.TriangleVertexPosition[1] = glm::vec4(0.5f, height / 2.0f, 0.0f, 1.0f);      // 上
		s_Data.TriangleVertexPosition[2] = glm::vec4(-0.5f, height / 2.0f, 0.0f, 1.0f);      // 右下

		
		s_Data.TriangleIndicesCount = 0;

	}



	void Renderer2D::Shutdown()
	{
		RG_PROFILE_FUNCTION();
	}

	void Renderer2D::WindowResize(uint32_t width, uint32_t height)
	{
	}

	//////////////////////////////////////////////////BeginScene/////////////////////////////////////////////////
	void Renderer2D::BeginScene(Camera& camera, glm::mat4 transform)
	{
		RG_PROFILE_FUNCTION();
		BeginScene(camera.GetProjection() * glm::inverse(transform));

		
	}
	void Renderer2D::BeginScene(glm::mat4 projection, glm::mat4 transform)
	{
		RG_PROFILE_FUNCTION();
		BeginScene(projection * glm::inverse(transform));
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		RG_PROFILE_FUNCTION();
		BeginScene(camera.GetViewProjectionMatrix());
	}
	void Renderer2D::BeginScene(glm::mat4 viewprojection)
	{
		std::cout<<viewprojection<<" ";
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadMat4("u_ViewProjection", viewprojection);
		
		//quad
		s_Data.IndicesCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		//triangle
		s_Data.TriangleIndicesCount = 0;
		s_Data.TriangleVertexBufferPtr = s_Data.TriangleVertexBufferBase;

		s_Data.TextureSlotsIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		if (s_Data.IndicesCount > 0)
		{
			s_Data.QuadVertexArray->Bind();
			uint32_t QuadDataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
			s_Data.QuadVertexArray->SetData(s_Data.QuadVertexBufferBase, QuadDataSize);
			Flush();
		}

		if (s_Data.TriangleIndicesCount > 0)
		{
			uint32_t TriangleDataSize = (uint8_t*)s_Data.TriangleVertexBufferPtr - (uint8_t*)s_Data.TriangleVertexBufferBase;
			s_Data.TriangleVertexArray->SetData(s_Data.TriangleVertexBufferBase, TriangleDataSize);
			FlushTriangles();
		}
	}


	void Renderer2D::Flush()
	{
		for (int i = 0; i < s_Data.TextureSlotsIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
		
		for (int i = 0; i < 8; i++)
		{
			std::cout << s_Data.QuadVertexBufferBase[i].Position << " ";
		}

		RendererCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndicesCount);
		
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushTriangles()
	{
		for (int i = 0; i < s_Data.TextureSlotsIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RendererCommand::DrawIndexed(s_Data.TriangleVertexArray, s_Data.TriangleIndicesCount);

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

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		DrawQuad(transform, color);

	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel, glm::vec4 tintColor)
	{
		Renderer2D::DrawQuad({ position.x, position.y, 0.0f }, size, texture, texZoomLevel, tintColor);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel, glm::vec4 tintColor)
	{
		RG_PROFILE_FUNCTION();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		DrawQuad(transform, texture, texZoomLevel, tintColor);

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
	void Renderer2D::DrawQuad(glm::mat4 transform, glm::vec4 color)
	{
		DrawQuad(transform, color, s_Data.QuadVertexPosition);
	}
	void Renderer2D::DrawQuad(glm::mat4 transform, glm::vec4 color, glm::vec4* vertexPosition)
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


		s_Data.QuadVertexBufferPtr->Position = transform * vertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * vertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * vertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * vertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TexZoomLevel = texZoomLevel;
		s_Data.QuadVertexBufferPtr++;

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCount++;

	}
	void Renderer2D::DrawQuad(glm::mat4 transform, Ref<Texture2D> texture, float texZoomLevel, glm::vec4 tintColor)
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
	
	
	void Renderer2D::DrawTriangle(glm::mat4 transform, glm::vec4 color)
	{
		DrawTriangle(transform, color, s_Data.TriangleVertexPosition);
	}

	void Renderer2D::DrawTriangle(glm::mat4 transform, glm::vec4 color, glm::vec4* vertexPosition)
	{
		//TODO ： 记录状态

		float texIndex = 0.0f;
		float texZoomLevel = 1.0f;
		glm::vec2 texCoords[3] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{0.5f, 1.0f}  // 简单的三角形纹理坐标
		};

		// 添加三角形的3个顶点
		for (int i = 0; i < 3; i++)
		{
			s_Data.TriangleVertexBufferPtr->Position = transform * vertexPosition[i];
			std::cout << s_Data.TriangleVertexBufferPtr->Position << " ";
			s_Data.TriangleVertexBufferPtr->Color = color;
			s_Data.TriangleVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.TriangleVertexBufferPtr->TexIndex = texIndex;
			s_Data.TriangleVertexBufferPtr->TexZoomLevel = texZoomLevel;
			s_Data.TriangleVertexBufferPtr++;


		}

		std::cout << "\n";
		// 三角形使用3个索引
		s_Data.TriangleIndicesCount += 3;
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