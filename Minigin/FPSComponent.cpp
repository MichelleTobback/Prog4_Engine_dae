#include "FPSComponent.h"

#include "Time.h"
#include "TextObject.h"
#include "GameObject.h"

void dae::FPSComponent::Update(GameObject* pGameObject)
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

	if (pGameObject->HasComponent<TextComponent>())
	{
		std::string text{"FPS : "};
		text.append(std::to_string(m_FPS));

		pGameObject->GetComponent<TextComponent>().SetText(text);
	}
}
