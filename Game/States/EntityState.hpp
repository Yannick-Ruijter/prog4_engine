#pragma once
#include "Entity.hpp"
#include "Observer.hpp"
#include <memory>
namespace dae {
class Entity;
class EntityState {
public:
  EntityState(EntityState const &other) = default;
  EntityState(EntityState &&other) = default;
  EntityState &operator=(EntityState const &other) = default;
  EntityState &operator=(EntityState &&other) = default;

  virtual ~EntityState();
  virtual std::unique_ptr<EntityState> HandleInput();
  virtual void Update();
  virtual void OnEnter();
  virtual void OnExit();

protected:
  EntityState(Entity &player);
  Entity *m_Entity;
};
} // namespace dae
