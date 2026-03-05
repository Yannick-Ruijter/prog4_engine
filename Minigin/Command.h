#pragma once
namespace dae
{
	class GameObject;
	class MoveComponent;
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
		MoveObjectCommand(GameObject& object);
		void Execute() override;

	private:
		MoveComponent* m_MoveComponent;
	};
}