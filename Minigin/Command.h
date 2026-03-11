#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class MoveComponent;
	class PlayerInput;
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject& object);
		virtual ~GameObjectCommand() = default;
	protected:
		GameObject* GetGameObject() const;
	private:
		GameObject* m_GameObject;
	};

	enum class MoveDirection;
	class MoveObjectCommand : public GameObjectCommand
	{
	public:
		MoveObjectCommand(GameObject& object, MoveDirection direction);
		void Execute() override;
		~MoveObjectCommand() override = default;
	private:
		MoveDirection m_MoveDirection;
		MoveComponent* m_MoveComponent{ nullptr };
	};
}