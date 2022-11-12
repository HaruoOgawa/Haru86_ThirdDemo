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
		CubeTrail(int SegmentFuncIndex, int BufferIndexTrailGroup, int BufferIndexTrailSegment,
			int NumOfSegmentThreads, int NumOfGroupThreads, int LineSegment, int TrailNum, float ParticleLife);
		virtual ~CubeTrail() = default;

		void Update(float time);
		void Draw();
	};
}