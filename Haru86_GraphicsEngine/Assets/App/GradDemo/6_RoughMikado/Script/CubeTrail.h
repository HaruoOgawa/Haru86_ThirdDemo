#pragma once
#include <memory>
class MeshRendererComponent;

namespace app
{
	class TrailRenderer;

	class CubeTrail
	{
		std::shared_ptr<MeshRendererComponent> m_InstancedCube;
		std::shared_ptr<TrailRenderer> m_TrailRenderer;
		int m_CubeNum;
	public:
		CubeTrail();
		virtual ~CubeTrail() = default;

		void Update(float time);
		void Draw();
	};
}