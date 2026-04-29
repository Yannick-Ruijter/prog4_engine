

#include "Command.h"
namespace dae
{
	class GameObject;
	class PlayerInput;
	class TransformComponent;
	class PlayerStateComponent;
	enum class MoveDirection
	{
		Up,
		Down,
		Left,
		Right,
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

	class TimeManager;
	class MoveObjectCommand : public GameObjectCommand
	{
	public:
		MoveObjectCommand(GameObject& object, MoveDirection direction, float speed = 100.f);
		virtual void Execute() override;
		virtual void StopExecution() override {};
		~MoveObjectCommand() override = default;
	private:
		glm::vec3 m_MoveDir{};
		TransformComponent* m_TransformComponent{ nullptr };
		TimeManager* m_TimeManager{ nullptr };
		float m_Speed{};

	};

	class Subject;
	class MovePlayerCommand : public MoveObjectCommand {
	public:
		MovePlayerCommand(GameObject& object, MoveDirection direction, PlayerStateComponent* state, float speed = 100.f);
		void Execute() override;
		void StopExecution() override;
		Subject* GetSubject() const;
	private:
		PlayerStateComponent* m_PlayerState{ nullptr };
		std::unique_ptr<Subject> m_OnPlayerStartedMove;
	};

	class HealthComponent;
	class DamagePlayer : public GameObjectCommand
	{
	public:
		DamagePlayer(GameObject& object, GameObject& target);
		void Execute() override;
		virtual void StopExecution() override {};
		~DamagePlayer() override = default;
	private:
		HealthComponent* m_TargetHealthComponent{ nullptr };
	};

	class PickUpItemCommand : public GameObjectCommand
	{
	public:
		PickUpItemCommand(GameObject& object);
		void Execute() override;
		virtual void StopExecution() override {};
		~PickUpItemCommand() override = default;
		Subject* GetSubject() const;
	private:
		std::unique_ptr<Subject> m_PlayerPickedUpItemEvent{ nullptr };

	};
}