#include "EntityState.hpp"

#include "Entity.hpp"
using namespace dae;

dae::EntityState::~EntityState() { OnExit(); }

std::unique_ptr<EntityState> dae::EntityState::HandleInput() { return nullptr; }

void dae::EntityState::Update() {}

void dae::EntityState::OnEnter() {}

void dae::EntityState::OnExit() {}

dae::EntityState::EntityState(Entity &entity) : m_Entity{&entity} { OnEnter(); }
