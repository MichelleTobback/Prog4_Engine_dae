#include "BurgerTime.h"

#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"

#include "Input/Input.h"
#include "Managers/SceneManager.h"
#include "States/GameModes/BTGameMode.h"
#include "States/GameModes/BTSinglePlayerGameMode.h"
#include "Core/BitFlag.h"

size_t dae::BurgerTime::CreateAnimClip(SpriteAtlasComponent* pAtlas, SpriteAnimatorComponent* pAnimator, const glm::u32vec4& rect, bool flip, float spriteSize, float speed)
{

    std::vector<SpriteAnimFrame> frames{};

    const glm::vec2 pos{ rect.x * spriteSize, rect.y * spriteSize };
    const glm::vec2 size{ pos.x + rect.z * spriteSize, pos.y + rect.w * spriteSize };
    for (float x{ pos .x}; x < size.x; x += spriteSize)
    {
        for (float y{ pos.y }; y < size.y; y += spriteSize)
        {
            SpriteComponent* pSprite{ pAtlas->GetSprite(pAtlas->AddSprite(x, y, spriteSize, spriteSize, flip)) };
            frames.push_back(SpriteAnimFrame{ pSprite });
        }
    }
    return pAnimator->AddAnimClip(frames, speed);
}

bool dae::BurgerTime::IsIngredient(SpawnID id)
{
    switch (id)
    {
    case BurgerTime::SpawnID::BottomBun:
    case BurgerTime::SpawnID::TopBun:
    case BurgerTime::SpawnID::Lettuce:
    case BurgerTime::SpawnID::Tomato:
    case BurgerTime::SpawnID::Cheese:
    case BurgerTime::SpawnID::Patty:
        return true;
        break;

    default:
        return false;
    }
}

void dae::BurgerTime::CreateDebugInput()
{
    auto& input{ Input::GetInstance().GetCommandHandler() };
    {
        auto& commandBinding{ input.AddValueBinding(static_cast<uint32_t>(InputID::ChangeScene)) };
        dae::InputCommand::ICDevices binding{ };
        BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
        binding.keyboard.Key = Keyboard::KeyCode::Num_1;
        binding.keyboard.State = Keyboard::KeyState::Pressed;
        commandBinding.Add(0.f, binding);

        binding.keyboard.Key = Keyboard::KeyCode::Num_2;
        commandBinding.Add(1.f, binding);

        binding.keyboard.Key = Keyboard::KeyCode::Num_3;
        commandBinding.Add(2.f, binding);
    }
    input.BindValueCommand(static_cast<uint32_t>(BurgerTime::InputID::ChangeScene), std::make_shared<InputCommand::ScalarCommand>([](float i)
        {
            static int current{-1};
            int index{ static_cast<int>(i) };
            if (current != index)
            {
                current = index;
                GameState::GetInstance().SetGameMode(std::make_shared<BTSinglePlayerGameMode>(index));
            }
        }));
}

void dae::BurgerTime::OpenMenu()
{
}

void dae::BurgerTime::OpenLevel()
{
}

void dae::BurgerTime::CreateInputBindings()
{
    auto& input{ Input::GetInstance().GetCommandHandler() };
    //Move
    {
        auto& commandBinding{ input.AddAxisBinding(static_cast<uint32_t>(InputID::Move)) };
        dae::InputCommand::ICDevices binding{};
        BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
        binding.keyboard.Key = Keyboard::KeyCode::A;
        binding.keyboard.State = Keyboard::KeyState::Down;
        BitFlag::Set(binding.flags, InputCommand::ICFlag::ControllerButton, true);
        binding.controller.Button = dae::Controller::ControllerButton::DPadLeft;
        binding.controller.ButtonState = dae::Controller::ControllerButtonState::Pressed;
        binding.controller.ControllerID = 0;
        commandBinding.Add({ -1.f, 0.f }, binding);

        binding.keyboard.Key = Keyboard::KeyCode::D;
        binding.controller.Button = dae::Controller::ControllerButton::DPadRight;
        commandBinding.Add({ 1.f, 0.f }, binding);

        binding.keyboard.Key = Keyboard::KeyCode::W;
        binding.controller.Button = dae::Controller::ControllerButton::DPadUp;
        commandBinding.Add({ 0.f, -1.f }, binding);

        binding.keyboard.Key = Keyboard::KeyCode::S;
        binding.controller.Button = dae::Controller::ControllerButton::DPadDown;
        commandBinding.Add({ 0.f, 1.f }, binding);
    }   
    //Throw
    {
        auto& commandBinding{ input.AddActionBinding(static_cast<uint32_t>(InputID::Throw)) };
        dae::InputCommand::ICDevices& binding{ commandBinding.deviceBinding };
        BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
        binding.keyboard.Key = Keyboard::KeyCode::P;
        binding.keyboard.State = Keyboard::KeyState::Pressed;
        BitFlag::Set(binding.flags, InputCommand::ICFlag::ControllerButton, true);
        binding.controller.Button = dae::Controller::ControllerButton::ButtonA;
        binding.controller.ButtonState = dae::Controller::ControllerButtonState::Down;
        binding.controller.ControllerID = 0;
    }
}
