#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
	class GameObject;
	class PlayerInput;
	class TransformComponent;
	enum class MoveDirection
	{
		Up,
		Down,
		Left,
		Right,
	};
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

	class TimeManager;
	class MoveObjectCommand : public GameObjectCommand
	{
	public:
		MoveObjectCommand(GameObject& object, MoveDirection direction, float speed = 100.f);
		void Execute() override;
		~MoveObjectCommand() override = default;
	private:
		glm::vec3 m_MoveDir{};
		TransformComponent* m_TransformComponent{ nullptr };
		TimeManager* m_TimeManager{ nullptr };
		float m_Speed{};

	};

	class HealthComponent;
	class DamagePlayer : public GameObjectCommand
	{
	public:
		DamagePlayer(GameObject& object, GameObject& target);
		void Execute() override;
		~DamagePlayer() override = default;
	private:
		GameObject* m_TargetObject{ nullptr };
		HealthComponent* m_TargetHealthComponent{ nullptr };
	};

	class Subject;
	class PickUpItemCommand : public GameObjectCommand
	{
	public:
		PickUpItemCommand(GameObject& object);
		void Execute() override;
		~PickUpItemCommand() override = default;
		Subject* GetSubject() const;
	private:
		std::unique_ptr<Subject> m_PlayerPickedUpItemEvent{ nullptr };

	};
}