#pragma once
#include <string>

namespace dae
{
	enum class WindowFlags
	{
		None = 0, 
		VSync = 1, 
		Initialized = 2
	};

	constexpr WindowFlags operator|(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	constexpr WindowFlags operator&(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	constexpr WindowFlags operator|=(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	constexpr WindowFlags operator&=(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	constexpr WindowFlags operator^=(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
	}

	struct WindowDesc
	{
		uint32_t width{};
		uint32_t height{};
		WindowFlags flags{ WindowFlags::VSync };
		std::string title{"Minigin"};
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetID() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class NullWindow final : public Window
	{
	public:
		NullWindow() = default;
		virtual ~NullWindow() override{}

		virtual void Init() override {}
		virtual void Shutdown() override{}

		virtual uint32_t GetID() const { return 0u; }
		virtual void Resize(uint32_t, uint32_t) override {}

		virtual uint32_t GetWidth() const override { return 0u; }
		virtual uint32_t GetHeight() const override { return 0u; }
	};
}