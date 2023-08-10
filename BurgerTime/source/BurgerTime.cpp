#include "BurgerTime.h"

#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"

#include "Input/Input.h"
#include "Managers/SceneManager.h"
#include "States/GameModes/BTGameMode.h"
#include "States/GameModes/BTSinglePlayerGameMode.h"

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
    static int current{};
    {
        Input::InputActionCommand actionCommand{ ActionCommand([]()
            {
                if (current != 0)
                {
                    current = 0;
                    GameState::GetInstance().SetGameMode(std::make_shared<BTSinglePlayerGameMode>(current));
                }
            }) };
        BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
        actionCommand.Keyboard.Key = Keyboard::KeyCode::Num_1;
        actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
        Input::GetInstance().AddActionCommand(actionCommand);
    }
    {
        Input::InputActionCommand actionCommand{ ActionCommand([]()
            {
                if (current != 1)
                {
                    current = 1;
                    GameState::GetInstance().SetGameMode(std::make_shared<BTSinglePlayerGameMode>(current));
                }
            }) };
        BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
        actionCommand.Keyboard.Key = Keyboard::KeyCode::Num_2;
        actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
        Input::GetInstance().AddActionCommand(actionCommand);
    }
    {
        Input::InputActionCommand actionCommand{ ActionCommand([]()
            {
                if (current != 2)
                {
                    current = 2;
                    GameState::GetInstance().SetGameMode(std::make_shared<BTSinglePlayerGameMode>(current));
                }
            }) };
        BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
        actionCommand.Keyboard.Key = Keyboard::KeyCode::Num_3;
        actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
        Input::GetInstance().AddActionCommand(actionCommand);
    }
}

void dae::BurgerTime::OpenMenu()
{
}

void dae::BurgerTime::OpenLevel()
{
}
