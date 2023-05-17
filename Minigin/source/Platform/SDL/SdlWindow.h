#pragma once
#include "Core/Window.h"

#include <memory>

namespace dae
{
	class SdlWindow final : public Window
	{
	public:
		SdlWindow(const WindowDesc& desc);
		virtual ~SdlWindow() override;

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual uint32_t GetID() const override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		class SdlWindowImpl;

	private:
		std::unique_ptr<SdlWindowImpl> m_pImpl;
	};
}