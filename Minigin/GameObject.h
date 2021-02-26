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
		std::shared_ptr<ComponentType> AddComponent()
		{
			auto pComponent = std::make_shared<ComponentType>();
			m_Components.push_back(pComponent);
			m_Components[m_Components.size() - 1]->SetGameObject(this);
			m_Components[m_Components.size() - 1]->Initialize();
			return pComponent;
		}

		template<typename ComponentType>
		std::shared_ptr<ComponentType> GetComponent() const
		{
			auto component = std::find_if(m_Components.begin(), m_Components.end(),
				[](std::shared_ptr<BaseComponent> component)
				{
					return typeid(*component).name() == typeid(ComponentType).name();
				});
			if (component != m_Components.end())
				return std::dynamic_pointer_cast<ComponentType>(*component);
			return nullptr;
		}

		void SetTexture(const std::string& fileName);
		void SetPosition(float x, float y);

		GameObject() { Initialize(); };
		~GameObject()
		{

		};
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		std::shared_ptr<Transform> m_pTransformComponent;
		std::shared_ptr<RenderComponent> m_pRenderComponent;
		std::shared_ptr<TextureComponent> m_pTextureComponent;
	};
}
