#define _USE_MATH_DEFINES
#include "GameObject.hpp"
#include "ObjectOrbitter.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include <cmath>

dae::ObjectOrbitter::ObjectOrbitter(GameObject &owner, float orbitOffset, float orbitSpeed)
    : Component(owner),
      m_OrbitOffset{orbitOffset},
      m_OrbitSpeed{orbitSpeed},
      m_TimeManager{&TimeManager::GetInstance()} {
}

void dae::ObjectOrbitter::Update() {
    m_CurrentAngle +=
        m_OrbitSpeed * m_TimeManager->GetDeltaTime() - static_cast<float>((m_CurrentAngle > M_PI * 2) * M_PI * 2);
    glm::vec3 currentPos{m_OrbitOffset * cos(m_CurrentAngle), m_OrbitOffset * sin(m_CurrentAngle), 0.f};
    GetOwner()->GetComponent<Transform>()->SetLocalPosition(currentPos);
}
