#pragma once

#include "Command.hpp"
#include "ServiceProvider.hpp"
#include <glm/glm.hpp>
namespace dae
{
    class GameObject;
    class PlayerInput;
    class TransformComponent;
    class SoundSystem;
    class Subject;
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right,
    };

    class GameObjectCommand : public Command
    {
      public:
        GameObjectCommand(GameObject &object);
        virtual ~GameObjectCommand() = default;

      protected:
        GameObject *GetGameObject() const;

      private:
        GameObject *m_GameObject;
    };

    class TimeManager;
    class MoveObjectCommand : public GameObjectCommand
    {
      public:
        MoveObjectCommand(GameObject &object, Direction direction, float speed = 100.f);
        virtual void Execute() override;
        ~MoveObjectCommand() override = default;

      private:
        glm::vec2 m_MoveDir{};
        TransformComponent *m_TransformComponent{nullptr};
        TimeManager *m_TimeManager{nullptr};
        float m_Speed{};
    };

    class HealthComponent;
    class DamagePlayer : public GameObjectCommand
    {
      public:
        DamagePlayer(GameObject &object, GameObject &target);
        void Execute() override;
        ~DamagePlayer() override = default;

      private:
        HealthComponent *m_TargetHealthComponent{nullptr};
    };

    class PickUpItemCommand : public GameObjectCommand
    {
      public:
        PickUpItemCommand(GameObject &object);
        virtual void Execute() override;
        ~PickUpItemCommand() override = default;
        Subject *GetSubject() const;

      private:
        std::unique_ptr<Subject> m_PlayerPickedUpItemEvent{nullptr};
    };

    class ButtonComponent;
    class NavigateButtonCommand : public Command
    {
      public:
        NavigateButtonCommand(Direction dir);
        static void SetInitialButton(ButtonComponent *button);
        virtual void Execute() override;

      private:
        static ButtonComponent *CurrentButton;
        Direction m_NavigateDir;
    };
} // namespace dae
