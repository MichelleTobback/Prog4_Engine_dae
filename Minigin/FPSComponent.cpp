#include "FPSComponent.h"

#include "Time.h"
#include "TextObject.h"
#include "GameObject.h"

#include "ResourceManager.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner)
	: Component(pOwner)
{
	if (!pOwner->HasComponent<TextComponent>())
	{
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		m_pTextComponent = pOwner->AddComponent<TextComponent>("", font);
	}
	else
		m_pTextComponent = pOwner->GetComponent<TextComponent>();
}

void dae::FPSComponent::Update()
{
	//FPS LOGIC
	m_FPSTimer += Time::GetInstance().GetDeltaTime();
	++m_FPSCount;
	if (m_FPSTimer > 1000.f)
	{
		m_FPS = m_FPSCount;
		m_FPSCount = 0;
		m_FPSTimer = 0.0f;
	}

	if (m_pTextComponent)
	{
		std::string text{"FPS : "};
		text.append(std::to_string(m_FPS));

		m_pTextComponent->SetText(text);
	}
}
