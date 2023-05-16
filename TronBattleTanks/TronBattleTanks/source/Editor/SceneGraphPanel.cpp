#include "SceneGraphPanel.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Prefabs.h"
#include "Component/SpriteAtlasComponent.h"
#include "Renderer/Texture2D.h"

#include "../3rdParty/imgui-master/imgui.h"

dae::SceneGraphPanel::SceneGraphPanel(GameObject* pOwner)
	: ImGuiComponent(pOwner, "Scene graph")
{
}

namespace dae
{
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, GameObject* pGameObject, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (pGameObject->HasComponent<T>())
		{
			auto component = pGameObject->GetComponent<T>();

			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			//if (ImGui::Button("+"))
			//{
			//	ImGui::OpenPopup("ComponentSettings");
			//}

			//bool removeComponent = false;
			//if (ImGui::BeginPopup("ComponentSettings"))
			//{
			//	if (ImGui::MenuItem("Remove component"))
			//		removeComponent = true;
			//
			//	ImGui::EndPopup();
			//}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			//if (removeComponent)
			//	pGameObject->RemoveComponent<T>();
		}
	}
}
void dae::SceneGraphPanel::OnImGuiRender()
{

	SceneManager& sceneManager{ SceneManager::GetInstance() };
	Scene* pScene{ sceneManager.GetCurrent() };

	if (ImGui::BeginMenu("File", true))
	{
		const size_t bufferSize{ 50 };
		static char buffer[bufferSize]{};
		ImGui::InputText("Path", &buffer[0], bufferSize);
		bool selected{ false };
		if (ImGui::MenuItem("Save", nullptr, &selected, true))
		{
			sceneManager.SaveScene(*pScene, buffer);
		}
		ImGui::EndMenu();
	}

	auto& pObjects{ pScene->GetObjects() };
	for (auto& [uuid, pGameObject] : pObjects)
	{
		if (uuid != GetUUID() && pGameObject->IsRoot())
			RenderGameObjectNode(pGameObject.get());
	}

	if (ImGui::BeginPopupContextWindow(0, 1))
	{
		AddGameObjectPopUp(pScene, nullptr);
		ImGui::EndPopup();
	}

	ImGui::End();
	ImGui::Begin("Components");

	if (m_SelectedGameObject != nullptr)
		DrawComponents(m_SelectedGameObject);
}

void dae::SceneGraphPanel::RenderGameObjectNode(GameObject* pGameObject)
{
	UUID uuid{ pGameObject->GetUUID() };
	ImGuiTreeNodeFlags flags{ ((m_SelectedGameObject != nullptr && m_SelectedGameObject->GetUUID() == uuid) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow};
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened{ ImGui::TreeNodeEx((void*)uint64_t(uuid), flags, std::to_string(uuid).c_str()) };

	if (ImGui::IsItemClicked())
	{
		m_SelectedGameObject = pGameObject;
	}

	bool deleted{ false };
	if (ImGui::BeginPopupContextItem())
	{
		AddGameObjectPopUp(pGameObject->GetScene(), pGameObject);

		ImGui::Separator();

		if (ImGui::MenuItem("Delete GameObject"))
		{
			deleted = true;
		}
		ImGui::EndPopup();
	}

	if (opened)
	{
		//TransformComponent& transform{ pGameObject->GetTransform() };

		for (GameObject* child : pGameObject->GetChildren())
		{
			RenderGameObjectNode(child);
		}
		ImGui::TreePop();
	}

	if (deleted)
		pGameObject->Destroy();
}

void dae::SceneGraphPanel::AddGameObjectPopUp(Scene* pScene, GameObject* pParent)
{
	if (ImGui::MenuItem("Create empty GameObject"))
	{
		pScene->Instantiate(pParent);
	}
}

void dae::SceneGraphPanel::DrawComponents(GameObject* pGameObject)
{
	DrawComponent<TransformComponent>("Transform", pGameObject, [](TransformComponent* component)
		{
			glm::vec3 position{ component->GetLocalPosition() };
			if (ImGui::DragFloat3("Translation", &position[0]))
				component->SetLocalPosition(position);

			float rotation{ component->GetLocalRotation() };
			if (ImGui::DragFloat("Rotation", &rotation))
				component->SetLocalRotation(rotation);
		});

	DrawComponent<SpriteAtlasComponent>("Sprite Atlas", pGameObject, [this](SpriteAtlasComponent* component)
		{
			auto& pSprites{ component->GetSprites() };
			for (int i{}; i < pSprites.size(); ++i)
			{
				const ImGuiTreeNodeFlags treeNodeFlags{ ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding };
				
				std::string spriteName{ std::string("sprite ") + std::to_string(i) };
				std::string spriteId{ "Sprite Atlas" + spriteName };
				bool open = ImGui::TreeNodeEx((void*)spriteId.c_str(), treeNodeFlags, spriteName.c_str());

				if (open)
				{
					ImGui::TreePop();
				}
			}

			if (ImGui::Button("Add sprites"))
			{
				component->AddSprite();
			}

			if (component->GetTexture().GetUUID() != component->GetUUID())
				DrawTextureComponent(component->GetTexture().GetOwner());
		});

	DrawSpriteComponent(pGameObject);
	DrawTextureComponent(pGameObject);

	AddComponentsPopUp(pGameObject);
}

void dae::SceneGraphPanel::DrawSpriteComponent(GameObject* pSprite, int index)
{
	DrawComponent<SpriteComponent>(std::string("Sprite " + std::to_string(index)).c_str(), pSprite, [](SpriteComponent* component)
		{
			glm::vec4 source{ component->GetSource() };
			if (ImGui::DragFloat4("Source", &source[0]))
				component->SetSource(source);
		});
}

void dae::SceneGraphPanel::AddComponentsPopUp(GameObject* pGameObject)
{
	std::string pupUpId{ std::to_string(pGameObject->GetUUID()) + "AddComponent" };
	if (ImGui::BeginPopupContextItem(pupUpId.c_str()))
	{
		if (ImGui::MenuItem("Add SpriteAtlasComponent"))
		{
			TextureComponent* pTexture{ pGameObject->AddComponent<TextureComponent>() };
			pGameObject->AddComponent<SpriteAtlasComponent>(pTexture);
		}

		ImGui::EndPopup();
	}
}

void dae::SceneGraphPanel::DrawTextureComponent(GameObject* pTexture)
{
	DrawComponent<TextureComponent>("Texture ", pTexture, [](TextureComponent* component)
		{
			if (component->HasTexture())
			{
				std::string path{ component->GetTexture()->GetPath() };
				std::string pathText{ "Path : " };
				pathText.append(path);
				ImGui::Text(pathText.c_str());
			}
			//if (ImGui::Button("Locate texture"))
			//{
			//	
			//}
			const size_t bufferSize{ 50 };
			static char buffer[bufferSize]{};
			ImGui::InputText("Path", &buffer[0], bufferSize);
			if (ImGui::Button("Load texture"))
			{
				component->SetTexture(buffer);
			}
		});
}

