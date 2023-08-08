#include "BurgerTime.h"

#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"

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
