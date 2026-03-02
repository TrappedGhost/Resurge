#pragma once
namespace Resug
{
	struct FrameBufferSrecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};
	class FrameBuffer
	{
	public:
		virtual const FrameBufferSrecification& GetSpecification() const = 0;
		virtual uint32_t GetRendererID() = 0;
		virtual uint32_t GetColorAttachmentRendererID() = 0;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSrecification& spec);

	};
}
