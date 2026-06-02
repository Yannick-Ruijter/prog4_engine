#include "GameObject.hpp"
#include "Health.hpp"
#include "Subject.hpp"
#include "sdbm_hash.hpp"
using namespace dae;
Health::Health(GameObject &owner, std::unique_ptr<Subject> &&liveLostEvent, int numberOfLives)
    : Component{owner}, m_NumberOfLives{numberOfLives}, m_LiveLostEvent{std::move(liveLostEvent)} {
}

dae::Health::~Health() = default;

void Health::Damage() {

    m_NumberOfLives -= 1;
    if (m_LiveLostEvent.get())
        m_LiveLostEvent->NotifyObservers("PlayerDeath"_h, GetOwner());
}

int dae::Health::GetNumberOfLives() const {
    return m_NumberOfLives;
}

Subject *dae::Health::GetSubject() const {
    return m_LiveLostEvent.get();
}
