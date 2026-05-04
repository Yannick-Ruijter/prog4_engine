
#include "PlayerAnimationComponent.hpp"
#include "GameObject.hpp"
#include "sdbm_hash.hpp"
#include <nlohmann/json.hpp>
using namespace dae;

PlayerAnimationComponent::PlayerAnimationComponent(GameObject &owner)
    : Component(owner)
{
}
