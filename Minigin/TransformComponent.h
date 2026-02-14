#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class TransformComponent final
	{
	public:
		static void AddToGameObject(GameObject* parent);
		static TransformComponent* GetFromObject(GameObject* object);

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);
	private:
		glm::vec3 m_position;
		friend GameObject;
	};
}
