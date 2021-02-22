#pragma once
#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "TextureComponent.h"
#include "RenderComponent.h"

namespace dae
{
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Initialize();
		void Update();
		void Render() const;

		template<typename ComponentType>
		ComponentType* AddComponent()
		{
			auto pComponent = new ComponentType();
			m_Components.push_back(pComponent);
			m_Components[m_Components.size() - 1]->SetGameObject(this);
			m_Components[m_Components.size() - 1]->Initialize();
			return pComponent;
		}

		template<typename ComponentType>
		ComponentType* GetComponent() const
		{
			auto component = std::find_if(m_Components.begin(), m_Components.end(),
				[](BaseComponent* component)
				{
					return typeid(*component).name() == typeid(ComponentType).name();
				});
			if (component != m_Components.end())
				return (ComponentType*)*component;
			return nullptr;
		}

		void SetTexture(const std::string& fileName);
		void SetPosition(float x, float y);

		GameObject() { Initialize(); };
		~GameObject()
		{
			for (auto element: m_Components)
			{
				delete element;
			}
		};
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<BaseComponent*> m_Components;
		Transform* m_pTransformComponent;
		RenderComponent* m_pRenderComponent;
		TextureComponent* m_pTextureComponent;
	};
}
