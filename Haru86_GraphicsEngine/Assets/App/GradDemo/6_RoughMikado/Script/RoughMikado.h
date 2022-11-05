#pragma once
#include <memory>

class MeshRendererComponent;

namespace roughmikado
{
	class CubeTrail;

	class RoughMikado
	{
		std::shared_ptr<MeshRendererComponent> m_TrailAura;
		std::shared_ptr<CubeTrail> m_TrailCube;
	public:
		RoughMikado();
		~RoughMikado() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}
