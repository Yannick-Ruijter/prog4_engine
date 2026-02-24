#pragma once
#include <string>
#include <memory>
#include <vector>
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include <SDL3/SDL.h>
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "Component.h"
#include <utility>

namespace dae
{
	class Font;
	class GameObject final 
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		void SetParent(GameObject* parent, bool keepCoordinates);
		GameObject* GetParent() const;

		glm::vec3& GetWorldPosition() const;

		template <typename T, typename ...Args>
		void AddComponent(Args&& ...args)
		{
			m_MyComponents.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
		};

		template <typename T>
		void RemoveComponent()
		{
			T* toBeRemoved = GetComponent<T>();
			m_MyComponents.erase(
				std::remove_if(
					m_MyComponents.begin(),
					m_MyComponents.end(),
					[toBeRemoved](const auto& ptr) { return ptr.get() == toBeRemoved; }
				),
				m_MyComponents.end()
			);
		};

		template <typename T>
		T* GetComponent() const
		{
			for (auto const& component : m_MyComponents)
			{
				T* castedComponent = dynamic_cast<T*>(component.get());
				if (castedComponent != nullptr) return castedComponent;
			}
			return nullptr;
		};


		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::unique_ptr<Component>> m_MyComponents;
		std::vector<GameObject*> m_Children{};
		GameObject* m_Parent{ nullptr };
		bool m_UpdatePosition{ true };

		bool IsChild(GameObject* object) const;
		void RemoveChild(GameObject* object);
		void AddChild(GameObject* object);

		void SetLocalPosition(glm::vec3 const& pos);
		void SetPositionDirty();
	};
}
