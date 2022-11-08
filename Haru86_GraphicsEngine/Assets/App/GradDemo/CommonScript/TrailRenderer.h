#pragma once
#include <memory>
#include <string>
class MeshRendererComponent;
class ComputeBuffer;
class Material;

namespace app
{
	struct SGroup
	{
		float param[4]; // vec4(now_segment, life, IsPermitUpdate, 0.0)
		SGroup() : param{ 0.0f, 0.1f, 0.0f, 0.0f }
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
		STrs(int g, int i) :
			pos{ 0.0f,0.0f,0.0f,0.0f }, rot{ 0.0f,0.0f,0.0f,0.0f }, scl{ 0.0f,0.0f,0.0f,0.0f }, tan{ 0.0f,0.0f,0.0f,0.0f },
			param{ g, i, 0, 0 }
		{
		}
	};

	class TrailRenderer
	{
		std::shared_ptr<ComputeBuffer> m_TrailGroupBuffer;
		std::shared_ptr<ComputeBuffer> m_TrailSegmentBuffer;
		std::shared_ptr<Material> m_TrailGroupCS;
		std::shared_ptr<Material> m_TrailSegmentCS;
#ifdef _DEBUG
		std::shared_ptr<MeshRendererComponent> m_TrailDebug;
#endif // _DEBUG
		int m_SegmentFuncIndex;
		int m_NumOfThreads;
		int m_LineSegment;
		int m_TrailNum;
		
		int m_BufferIndexTrailGroup;
		int m_BufferIndexTrailSegment;
	private:
		void InitBuffer();
	public:
		TrailRenderer(int SegmentFuncIndex, int BufferIndexTrailGroup, int BufferIndexTrailSegment, int NumOfThreads, int LineSegment, int TrailNum);
		virtual ~TrailRenderer();

		void Start();
		void Update(float time);
		void Draw();

		int GetBufferIndexTrailGroup()const { return m_BufferIndexTrailGroup; }
		int GetBufferIndexTrailSegment()const { return m_BufferIndexTrailSegment; }
	};
}