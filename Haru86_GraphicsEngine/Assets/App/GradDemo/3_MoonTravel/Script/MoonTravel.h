#pragma once
#include <memory>

class MeshRendererComponent;

namespace app {
	class MoonTravel
	{
		std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
	public:
		MoonTravel();
		virtual ~MoonTravel() = default;

		void Update();
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}