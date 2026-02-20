#pragma once

namespace dae
{
	class GameObject;
	class Component {
	public:
		virtual ~Component() = default;
		virtual void Update(float deltaTime);
		virtual void Render() const;
	protected:
		Component(GameObject* owner);
		GameObject* GetOwner() const;
	private:
		GameObject* m_Owner;
	};
}