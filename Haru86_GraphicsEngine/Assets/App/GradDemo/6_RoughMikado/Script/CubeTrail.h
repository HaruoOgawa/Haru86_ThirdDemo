#pragma once
#include <memory>

class MeshRendererComponent;
class ComputeBuffer;
class Material;

namespace roughmikado
{
	struct SGroup
	{
		/*int	  now_segment;
		float life;
		int	  IsPermitUpdate;*/
		float param[4]; // vec4(now_segment, life, IsPermitUpdate, 0.0)
		SGroup() : param{0.0f, 0.1f, 0.0f, 0.0f} /*now_segment(0), life(0.1f), IsPermitUpdate(0)*/
		{
		}
	};

	struct STrs
	{
		float pos[4];
		float rot[4];
		float scl[4];
		float tan[4];
		int	  param[4]; // ivec4(group, my_segment, now_segment, 0)
		/*int	  group;
		int	  my_segment;
		int	  now_segment;*/
		STrs(int g, int i):
			pos{0.0f,0.0f,0.0f,0.0f}, rot{ 0.0f,0.0f,0.0f,0.0f }, scl{ 0.0f,0.0f,0.0f,0.0f }, tan{ 0.0f,0.0f,0.0f,0.0f }, 
			param{g, i, 0, 0}/*group(g), my_segment(i), now_segment(0)*/
		{
		}
	};

	class CubeTrail
	{
		std::shared_ptr<MeshRendererComponent> m_InstancedCube;
		
		std::shared_ptr<ComputeBuffer> m_TrailGroupBuffer;
		std::shared_ptr<ComputeBuffer> m_TrailSegmentBuffer;
		std::shared_ptr<Material> m_TrailGroupCS;
		std::shared_ptr<Material> m_TrailSegmentCS;

#ifdef _DEBUG
		std::shared_ptr<MeshRendererComponent> m_TrailDebug;
#endif // _DEBUG

		int m_NumOfThreads;
		int m_LineSegment;
		int m_TrailNum;
		int m_CubeNum;

		int m_BufferIndexTrailGroup;
		int m_BufferIndexTrailSegment;
	private:
		void InitBuffer();
	public:
		CubeTrail();
		virtual ~CubeTrail();

		void Start();
		void Update(float time);
		void Draw();
	};
}