#pragma once

#include"Resug/Renderer/FrameBuffer.h"
namespace Resug
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(FrameBufferSrecification spec);
		~OpenGLFrameBuffer();

		void Invalidate();
		virtual void Bind()const override;
		virtual void UnBind()const override;
		
		virtual const FrameBufferSrecification& GetSpecification() const override { return m_Spec; };

		virtual uint32_t GetRendererID() override { return m_RendererID; };
		virtual uint32_t GetColorAttachmentRendererID()override { return m_ColorAttachment; };


	private:
		FrameBufferSrecification m_Spec;
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
	};
}
