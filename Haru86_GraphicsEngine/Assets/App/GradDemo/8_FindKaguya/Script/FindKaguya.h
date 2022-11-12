#pragma once
#include <memory>

class MeshRendererComponent;

namespace app
{
	class CubeTrail;
	class FindKaguya
	{
		std::shared_ptr<MeshRendererComponent> m_Voxel;
		std::shared_ptr<CubeTrail> m_CubeTrail;
	public:
		FindKaguya();
		~FindKaguya() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}