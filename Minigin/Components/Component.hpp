#pragma once
namespace dae {
class GameObject;
class Component {
private:
  GameObject *m_Owner;

public:
  virtual ~Component() = default;
  Component(Component const &other) = delete;
  Component(Component &&other) = delete;
  Component &operator=(Component const &other) = delete;
  Component &operator=(Component &&other) = delete;
  virtual void Update();
  virtual void LateUpdate();
  virtual void Render() const;
  GameObject *GetOwner() const;

protected:
  Component(GameObject &owner);
};
} // namespace dae
