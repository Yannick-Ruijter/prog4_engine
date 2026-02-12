#pragma once
#include <memory>
#include <string>
namespace dae
{
	class Texture2D;
	class GameObject;
	class Transform;
	class RenderComponent final 
	{
	public:
		RenderComponent(GameObject* parent);
		~RenderComponent() = default;
		void Render(Texture2D* texture, Transform const& transform) const;

	private:
		GameObject* m_Parent;
		//std::shared_ptr<Texture2D> m_Texture{};

	};
}