#pragma once

#include "Command.hpp"
#include "ServiceProvider.hpp"
#include <glm/glm.hpp>
namespace dae {
class GameObject;
class PlayerInput;
class Transform;
class SoundSystem;
class Subject;
enum class Direction { Up, Down, Left, Right, Neutral };

class GameObjectCommand : public Command {
public:
  GameObjectCommand(GameObject &object);
  virtual ~GameObjectCommand() = default;

protected:
  GameObject *GetGameObject() const;

private:
  GameObject *m_GameObject;
};

class TimeManager;
class MoveObjectCommand : public GameObjectCommand {
public:
  MoveObjectCommand(GameObject &object, Direction direction,
                    float speed = 100.f);
  virtual void Execute() override;
  ~MoveObjectCommand() override = default;

private:
  glm::vec2 m_MoveDir{};
  Transform *m_Transform{nullptr};
  TimeManager *m_TimeManager{nullptr};
  float m_Speed{};
};

class Health;
class DamagePlayer : public GameObjectCommand {
public:
  DamagePlayer(GameObject &object, GameObject &target);
  void Execute() override;
  ~DamagePlayer() override = default;

private:
  Health *m_TargetHealthComponent{nullptr};
};

class PickUpItemCommand : public GameObjectCommand {
public:
  PickUpItemCommand(GameObject &object);
  virtual void Execute() override;
  ~PickUpItemCommand() override = default;
  Subject *GetSubject() const;

private:
  std::unique_ptr<Subject> m_PlayerPickedUpItemEvent{nullptr};
};

class Button;

class ButtonCommand : public Command {
public:
  static void SetInitialButton(Button *button);

protected:
  static Button *CurrentButton;
};
class NavigateButtonCommand : public ButtonCommand {
public:
  NavigateButtonCommand(Direction dir);
  virtual void Execute() override;

private:
  Direction m_NavigateDir;
};

class PressButtonCommand : public ButtonCommand {
public:
  virtual void Execute() override;
};

class TextDisplay;
class ScrollButtonInitials : public ButtonCommand {
public:
  ScrollButtonInitials(Direction dir);
  virtual void Execute() override;
  static void SetNameSelectionButton(Button *button);
  static std::string Text;
  static int CharIndex;

private:
  static Button *Target;
  static TextDisplay *TargetText;
  Direction m_Dir;
};

class ToggleSoundCommand : public Command {
public:
  virtual void Execute() override;
};

class GameManager;
class AdvanceSceneCommand : public Command {
public:
  AdvanceSceneCommand(GameManager *manager);
  virtual void Execute() override;

private:
  GameManager *m_Manager;
};
} // namespace dae
