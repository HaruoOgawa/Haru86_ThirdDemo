#pragma once
#include <memory>
#include <vector>

class MeshRendererComponent;

namespace app
{
	class CubeTrail;
	class FindKaguya
	{
		std::shared_ptr<MeshRendererComponent> m_Voxel;
	public:
		FindKaguya();
		~FindKaguya() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}