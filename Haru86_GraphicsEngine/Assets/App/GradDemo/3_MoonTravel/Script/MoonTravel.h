#pragma once
#include <memory>

class MeshRendererComponent;

namespace app {
	class MoonTravel
	{
		std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
		std::shared_ptr<MeshRendererComponent> m_RaySpaceShip;
	public:
		MoonTravel();
		virtual ~MoonTravel() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}