#include "BurgerTime.h"

#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"

#include "Input/Input.h"
#include "Managers/SceneManager.h"
#include "States/GameStates/BTGameMode.h"
#include "States/GameStates/BTSinglePlayerGameMode.h"
#include "Prefabs.h"
#include "GameManager.h"
#include "Core/BitFlag.h"
#include "Managers/ServiceLocator.h"
#include <iostream>

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
        auto& commandBinding{ input.AddActionBinding(static_cast<uint32_t>(InputID::ChangeScene)) };
        dae::InputCommand::ICDevices& binding{ commandBinding.deviceBinding };
        BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
        binding.keyboard.Key = Keyboard::KeyCode::Num_1;
        binding.keyboard.State = Keyboard::KeyState::Pressed;
    }
    input.BindActionCommand(static_cast<uint32_t>(BurgerTime::InputID::ChangeScene), std::make_shared<ActionCommand>([]()
        {
            BTGameMode* pGameMode{ dynamic_cast<BTGameMode*>(&GameManager::GetInstance().GetState()) };
            if (pGameMode)
            {
                pGameMode->OpenNextLevel();
            }
        }));
}

void dae::BurgerTime::LoadScenes()
{
    SceneManager& sceneManager{ SceneManager::GetInstance() };
    Scene& menu{ sceneManager.CreateScene("MainMenu", false) };
    Prefabs::CreateMenuObject(&menu);

    sceneManager.LoadScene("Scenes/BurgerTimeLevel1NoBurger.scene");
    sceneManager.LoadScene("Scenes/BurgerTimeLevel1.scene");
    sceneManager.LoadScene("Scenes/BurgerTimeLevel2.scene");
    sceneManager.LoadScene("Scenes/BurgerTimeLevel3.scene");

    //Prefabs::CreateTestLevel(&sceneManager.CreateScene("BurgerTimeLevel3", false));

    sceneManager.LoadScene("Scenes/BurgerTimeLevel1NoBurger.scene");
}

void dae::BurgerTime::CreateInputBindings()
{
    auto& input{ Input::GetInstance().GetCommandHandler() };
    //adjust volume
    {
        auto commandBinding{ input.AddValueBinding() };
        dae::InputCommand::ICDevices binding{};
        binding.controller.ControllerID = static_cast<unsigned int>(-1);
        BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
        binding.keyboard.State = Keyboard::KeyState::Down;

        binding.keyboard.Key = Keyboard::KeyCode::F10;
        commandBinding.second->Add(-1.f, binding);

        binding.keyboard.Key = Keyboard::KeyCode::F11;
        commandBinding.second->Add(1.f, binding);

        input.BindValueCommand(commandBinding.first, std::make_shared<InputCommand::ScalarCommand>([](float value)
            {
                SoundSystem& sound{ ServiceLocator::GetSoundSystem() };
                float volume{ sound.GetMasterVolume() + value};
                sound.SetMasterVolume(volume);
            }));
    }   
}

void dae::BurgerTime::PrintControls()
{
    std::cout << "=================================================\n";
    std::cout << "===========       controls         ==============\n";
    std::cout << "=================================================\n";
    std::cout << "1 skip level\n";
    std::cout << "F10 / F11 : change volume\n";
    std::cout << "WASD / controller arrow buttons : move\n";
    std::cout << "P / controller A button : throw pepper\n";
    std::cout << "=================================================\n";
}
