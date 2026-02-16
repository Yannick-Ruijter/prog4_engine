#pragma once

namespace dae
{
	class GameObject;
	class Component {
	public:
		Component(GameObject* parent);
		virtual void Update(float deltaTime);
		virtual void Render() const;
	protected:
		GameObject* m_Parent;
	};
}