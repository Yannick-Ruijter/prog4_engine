#pragma once
#include <vector>

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

	class MoveObjectCommand : public GameObjectCommand
	{
	public:
		MoveObjectCommand(GameObject& object, PlayerInput& input, uint32_t up, uint32_t down, uint32_t left, uint32_t right);
		void Execute() override;
		~MoveObjectCommand() override = default;
	private:
		MoveComponent* m_MoveComponent;
		PlayerInput* m_PlayerInput;
		std::vector<uint32_t> m_InputButtons;
	};
}