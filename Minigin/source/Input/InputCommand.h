#pragma once
#include "Input/Controller.h"
#include "Core/Command.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Core/BitFlag.h"

#include <glm/glm.hpp>
#include <map>

namespace dae
{
	namespace InputCommand
	{
		enum class ICFlag
		{
			None = 0,
			Keyboard = 1, ControllerButton = 2, ControllerThumbStick = 4, MouseButton = 8, MouseMoved = 16
		};

		struct ICController
		{
			unsigned int ControllerID{};
			Controller::ControllerButton Button{};
			Controller::ControllerButtonState ButtonState{};
		};

		struct ICKeyboard
		{
			Keyboard::KeyCode Key{};
			Keyboard::KeyState State{};
		};

		struct ICMouse
		{
			Mouse::MouseButton button{};
			Mouse::MouseButtonState state{};
		};

		struct ICDevices
		{
			ICController controller{};
			ICKeyboard keyboard{};
			ICMouse mouse{};
			ICFlag flags{ ICFlag::None };
		};

		struct ICActionBinding
		{
			ICActionBinding() = default;
			ICDevices deviceBinding{};

		private:
			friend class ICHandler;
			std::vector<std::shared_ptr<ActionCommand>> pActions;
		};

		template <typename T>
		struct ICValueBinding
		{
			ICValueBinding() = default;
			std::vector<std::pair<ICDevices, T>> deviceBindings;

			ICDevices& Add(T value, const ICDevices& devices);

		private:
			friend class ICHandler;
			std::vector<std::shared_ptr<ValueCommand<T>>> pActions;
		};
		using ScalarCommand = ValueCommand<float>;
		using AxisCommand = ValueCommand<glm::vec2>;
		using ICScalarBinding = ICValueBinding<float>;
		using ICAxisBinding = ICValueBinding<glm::vec2>;

		class ICHandler final
		{
		public:
			ICHandler() = default;

			ICActionBinding& AddActionBinding(uint32_t id);
			ICScalarBinding& AddValueBinding(uint32_t id);
			ICAxisBinding& AddAxisBinding(uint32_t id);

			std::pair<uint32_t, ICActionBinding*> AddActionBinding();
			std::pair<uint32_t, ICScalarBinding*> AddValueBinding();
			std::pair<uint32_t, ICAxisBinding*> AddAxisBinding();

			void BindActionCommand(uint32_t id, const std::shared_ptr<ActionCommand>& actionCommand);
			void BindValueCommand(uint32_t id, const std::shared_ptr<ScalarCommand>& valueBinding);
			void BindAxisCommand(uint32_t id, const std::shared_ptr<AxisCommand>& axisBinding);

			void UnbindActionCommand(uint32_t id, const std::shared_ptr<ActionCommand>& actionCommand);
			void UnbindValueCommand(uint32_t id, const std::shared_ptr<ScalarCommand>& valueBinding);
			void UnbindAxisCommand(uint32_t id, const std::shared_ptr<AxisCommand>& axisBinding);

			void Handle();

		private:
			bool HandleControllerActionCommand(ICController& device, Controller* pController);
			bool HandleKeyboardActionCommand(ICKeyboard& device, Keyboard* pKeyboard);
			bool HandleMouseActionCommand(ICMouse& device, Mouse* pMouse);

			bool HandleControllerValueCommand(ICController& device, Controller* pController);
			bool HandleKeyboardValueCommand(ICKeyboard& device, Keyboard* pKeyboard);
			bool HandleControllerAxisCommand(ICController& device, Controller* pController);
			bool HandleKeyboardAxisCommand(ICKeyboard& device, Keyboard* pKeyboard);

			template <typename T>
			uint32_t FindAvailableID(const std::unordered_map<uint32_t, std::unique_ptr<T>>& commandMap);

			std::unordered_map<uint32_t, std::unique_ptr<ICActionBinding>> m_ActionCommands{};
			std::unordered_map<uint32_t, std::unique_ptr<ICScalarBinding>> m_ValueCommands{};
			std::unordered_map<uint32_t, std::unique_ptr<ICAxisBinding>> m_AxisCommands{};

		};
		template<typename T>
		inline ICDevices& ICValueBinding<T>::Add(T value, const ICDevices& devices)
		{
			deviceBindings.push_back(std::make_pair(devices, value));
			return deviceBindings.back().first;
		}
		template<typename T>
		inline uint32_t ICHandler::FindAvailableID(const std::unordered_map<uint32_t, std::unique_ptr<T>>& commandMap)
		{
			int lowestAvailableID{};
			while (commandMap.find(lowestAvailableID) != commandMap.end()) 
			{
				lowestAvailableID++;
			}
			return lowestAvailableID;
		}
}
}