#pragma once
#include <memory>
#include <vector>

class MeshRendererComponent;
namespace graphic { class RenderBuffer; }

namespace app
{
	class CubeTrail;
	class FindKaguya
	{
		std::shared_ptr<MeshRendererComponent> m_Voxel;
		//std::shared_ptr<CubeTrail> m_CubeTrail;
		std::vector<std::shared_ptr<graphic::RenderBuffer>> m_RenderBufferList;
		std::shared_ptr<MeshRendererComponent> m_RayTrail;
	public:
		FindKaguya();
		~FindKaguya() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}