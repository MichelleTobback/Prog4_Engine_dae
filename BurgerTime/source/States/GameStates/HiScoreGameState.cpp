#include "HiScoreGameState.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/UI/ButtonComponent.h"
#include "Component/TextComponent.h"
#include "Audio/AudioClip.h"
#include "GameManager.h"

dae::HiScoreGameState::HiScoreGameState()
    : GameState(1), m_pMusic{ std::make_unique<AudioClip>("Sounds/03 Level End.mp3") }
{
    
}

void dae::HiScoreGameState::OnEnter(Scene& scene)
{
    m_pMusic->Play();
    scene.GetGameObjectWithTag("hs")[0]->SetActive(true);
    ButtonComponent* pButton{ scene.GetGameObjectWithTag("MButton")[0]->GetComponent<ButtonComponent>() };
    pButton->GetOnReleasedDelegate() += std::bind(&HiScoreGameState::Next, this);

    auto pNames{ scene.GetGameObjectWithTag("hsname") };
    auto pScores{ scene.GetGameObjectWithTag("hsscore") };

    std::sort(pNames.begin(), pNames.end(), [](GameObject* pObj0, GameObject* pObj1)->bool
        {
            return pObj0->GetTransform().GetWorldPosition().y < pObj1->GetTransform().GetWorldPosition().y;
        });
    std::sort(pScores.begin(), pScores.end(), [](GameObject* pObj0, GameObject* pObj1)->bool
        {
            return pObj0->GetTransform().GetWorldPosition().y < pObj1->GetTransform().GetWorldPosition().y;
        });

    GameManager& gameManager{ GameManager::GetInstance() };
    const GameSaveData& saveData{ gameManager.GetSaveData() };
    for (size_t i{}; i < SaveGame::NUM_HISCORES; ++i)
    {
        const auto& hiScore{ saveData.hiScores[i] };
        const std::string scoreTxt{ std::to_string(hiScore.hiScore) };
        const std::string name{ !saveData.names.empty() ? saveData.names[hiScore.nameIndex] : " "};
        pNames[i]->GetComponent<TextComponent>()->SetText(name);
        pScores[i]->GetComponent<TextComponent>()->SetText(scoreTxt);
    }
}

void dae::HiScoreGameState::OnExit(Scene& scene)
{
    scene.GetGameObjectWithTag("hs")[0]->SetActive(false);
    ButtonComponent* pButton{ scene.GetGameObjectWithTag("MButton")[0]->GetComponent<ButtonComponent>() };
    pButton->GetOnReleasedDelegate().Clear();
}

void dae::HiScoreGameState::Next()
{
    auto& gameManager{ dae::GameManager::GetInstance() };
    gameManager.PopWhile([](size_t prevScene) {return prevScene != 0; });
}
