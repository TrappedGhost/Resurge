#pragma once

#include"Resug/Renderer/Buffer.h"
namespace Resug
{
	class VertexArray
	{
	public:
		~VertexArray() {}

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
		
	};

}