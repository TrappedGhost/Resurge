#pragma once

#include"Resug/Log.h"

#include"Resug/Renderer/VertexArray.h"
namespace Resug
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() {}
		virtual void Bind()const override;
		virtual void UnBind()const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; }
		virtual inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffers; }
	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffers;

	};
}

