#pragma once
#include <memory>

class MeshRendererComponent;
class ComputeBuffer;
class Material;

namespace roughmikado
{
	struct STrs
	{
		float pos[4];
		float rot[4];
		float scl[4];
		float tan[4];
		int	  index;
		int	  now_index;
		STrs(int i):
			pos{0.0f,0.0f,0.0f,0.0f}, rot{ 0.0f,0.0f,0.0f,0.0f }, scl{ 0.0f,0.0f,0.0f,0.0f }, tan{ 0.0f,0.0f,0.0f,0.0f },
			index(i), now_index(0)
		{
		}
	};

	class CubeTrail
	{
		std::shared_ptr<MeshRendererComponent> m_InstancedCube;
		
		std::shared_ptr<ComputeBuffer> m_TrailBaseBuffer;
		std::shared_ptr<Material> m_BaseTrailCS;

#ifdef _DEBUG
		std::shared_ptr<MeshRendererComponent> m_TrailDebug;
#endif // _DEBUG

		int m_LineSegment;
		int m_CubeNum;

		int m_BufferIndexTrailBase;
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