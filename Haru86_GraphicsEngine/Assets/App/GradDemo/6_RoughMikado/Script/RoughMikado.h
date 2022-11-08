#pragma once
#include <memory>

class MeshRendererComponent;

namespace app
{
	class CubeTrail;
	class AuraTrail;

	class RoughMikado
	{
		std::shared_ptr<CubeTrail> m_TrailCube;
		std::shared_ptr<AuraTrail> m_AuraTrail;
		std::shared_ptr<MeshRendererComponent> m_AuraBackRenderer;
	public:
		RoughMikado();
		~RoughMikado() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}
