#include "TrailRenderer.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG
namespace app
{
	TrailRenderer::TrailRenderer(const std::string& SegmentCS, int BufferIndexTrailGroup, int BufferIndexTrailSegment,
		int NumOfThreads, int LineSegment, int TrailNum) :
		m_TrailGroupBuffer(nullptr),
		m_TrailSegmentBuffer(nullptr),
		m_TrailGroupCS(nullptr),
		m_TrailSegmentCS(nullptr),
		m_NumOfThreads(NumOfThreads),
		m_LineSegment(LineSegment),
		m_TrailNum(TrailNum),
#ifdef _DEBUG
		m_TrailDebug(nullptr),
#endif // _DEBUG

		m_BufferIndexTrailGroup(BufferIndexTrailGroup),
		m_BufferIndexTrailSegment(BufferIndexTrailSegment)
	{
		// ïœêîÇÃèâä˙âª
		m_TrailGroupBuffer = std::make_shared<ComputeBuffer>(sizeof(SGroup) * m_TrailNum);
		m_TrailSegmentBuffer = std::make_shared<ComputeBuffer>(sizeof(STrs) * m_LineSegment * m_TrailNum);

		m_TrailGroupCS = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, "", "", "", "", "",
			"#version 430\n"
			"#define NUMTHREAD " + std::to_string(m_NumOfThreads) + "\n" +
			"#define GroupBufferBinding " + std::to_string(m_BufferIndexTrailGroup) + "\n" +
			std::string(
				#include "../CommonShader/TrailGroup.comp"
			)
		);

		m_TrailSegmentCS = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, "", "", "", "", "",
			"#version 430\n"
			"#define NUMTHREAD " + std::to_string(m_NumOfThreads) + "\n" +
			"#define GroupBufferBinding " + std::to_string(m_BufferIndexTrailGroup) + "\n" +
			"#define SegmentBufferBinding " + std::to_string(m_BufferIndexTrailSegment) + "\n" +
			SegmentCS
		);

#ifdef _DEBUG
		m_TrailDebug =std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::POINT,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::Standard_vert,
			shaderlib::Standard_frag,
			"#version 430\n"
			"#define SegmentBufferBinding " + std::to_string(m_BufferIndexTrailSegment) + "\n" +
			std::string(
				#include "../CommonShader/TrailDebug.geom"
			)
		);
#endif // _DEBUG

		// GPGPUÇÃèÄîı
		InitBuffer();

		// èââÒGPGPUÇÃé¿çs
		Start();
	}

	TrailRenderer::~TrailRenderer()
	{
		if (m_TrailGroupBuffer)
		{
			m_TrailGroupBuffer->DeleteBuffer();
			m_TrailGroupBuffer = nullptr;
		}

		if (m_TrailSegmentBuffer)
		{
			m_TrailSegmentBuffer->DeleteBuffer();
			m_TrailSegmentBuffer = nullptr;
		}
	}

	void TrailRenderer::InitBuffer()
	{
		{
			std::vector<SGroup> InitGroupData;
			std::vector<STrs> InitData;
			for (int g = 0; g < m_TrailNum; g++)
			{
				InitGroupData.push_back(SGroup());
				for (int s = 0; s < m_LineSegment; s++) { InitData.push_back(STrs(g, s)); }
			}

			m_TrailGroupBuffer->SetData<std::vector<SGroup>>(InitGroupData);
			m_TrailSegmentBuffer->SetData<std::vector<STrs>>(InitData);

			m_TrailGroupCS->SetBufferToCS(m_TrailGroupBuffer, m_BufferIndexTrailGroup);

			m_TrailSegmentCS->SetBufferToCS(m_TrailSegmentBuffer, m_BufferIndexTrailSegment);
			m_TrailSegmentCS->SetBufferToCS(m_TrailGroupBuffer, m_BufferIndexTrailGroup);

#ifdef _DEBUG
			m_TrailDebug->m_material->SetBufferToMat(m_TrailSegmentBuffer, m_BufferIndexTrailSegment);
#endif // _DEBUG
		}
	}

	void TrailRenderer::Start()
	{
		{
			m_TrailGroupCS->SetActive();
			m_TrailGroupCS->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
			m_TrailGroupCS->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
			m_TrailGroupCS->SetIntUniform("_TrailNum", m_TrailNum);
			m_TrailGroupCS->SetIntUniform("_LineSegment", m_LineSegment);
			m_TrailGroupCS->Dispatch(m_TrailNum / m_NumOfThreads, 1, 1);
		}

		{
			m_TrailSegmentCS->SetActive();
			m_TrailSegmentCS->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
			m_TrailSegmentCS->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
			m_TrailSegmentCS->SetIntUniform("_TrailNum", m_TrailNum);
			m_TrailSegmentCS->SetIntUniform("_LineSegment", m_LineSegment);
			m_TrailSegmentCS->Dispatch((m_TrailNum * m_LineSegment) / m_NumOfThreads, 1, 1);
		}
	}

	void TrailRenderer::Update(float time)
	{
		{
			m_TrailGroupCS->SetActive();
			m_TrailGroupCS->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
			m_TrailGroupCS->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
			m_TrailGroupCS->SetIntUniform("_TrailNum", m_TrailNum);
			m_TrailGroupCS->SetIntUniform("_LineSegment", m_LineSegment);
			m_TrailGroupCS->Dispatch((m_TrailNum) / m_NumOfThreads, 1, 1);
		}

		{
			m_TrailSegmentCS->SetActive();
			m_TrailSegmentCS->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
			m_TrailSegmentCS->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
			m_TrailSegmentCS->SetIntUniform("_TrailNum", m_TrailNum);
			m_TrailSegmentCS->SetIntUniform("_LineSegment", m_LineSegment);
			m_TrailSegmentCS->Dispatch((m_TrailNum * m_LineSegment) / m_NumOfThreads, 1, 1);
		}

#ifdef _DEBUG
		/*if (GraphicsMain::GetInstance()->m_ShowDebugLog)
		{
			//Console::Log("Showing DebugLog\n");
			std::vector<STrs> DebugData;
			DebugData.resize(m_TrailNum * m_LineSegment, STrs(0, 0));
			m_TrailSegmentBuffer->GetBufferData<STrs>(&DebugData[0], 0, DebugData.size());

			Console::Log("______________________________________\n");
			for (int i = 0; i< DebugData.size(); i++)
			{
				const auto& val = DebugData[i];
				Console::Log("Element: %d / group: %d / my_segment: %d / now_segment: %d / padding: %d / pos=> x:%f, y:%f, z:%f, w:%f\n",
					i, val.param[0], val.param[1], val.param[2], val.param[3], val.pos[0], val.pos[1], val.pos[2], val.pos[3]);
			}
		}

		if (GraphicsMain::GetInstance()->m_ShowDebugLog)
		{
			//Console::Log("Showing DebugLog\n");
			std::vector<SGroup> DebugData;
			DebugData.resize(m_TrailNum, SGroup());
			m_TrailGroupBuffer->GetBufferData<SGroup>(&DebugData[0], 0, DebugData.size());

			Console::Log("______________________________________\n");
			for (int i = 0; i< DebugData.size(); i++)
			{
				const auto& val = DebugData[i];
				Console::Log("Element: %d / now_segment: %f / life: %f / IsPermitUpdate: %f / padding: %f\n",
					i, val.param[0], val.param[1], val.param[2], val.param[3]);
			}
		}*/
#endif // _DEBUG
	}

	void TrailRenderer::Draw()
	{
#ifdef _DEBUG
		m_TrailDebug->Draw([&]() {
			m_TrailDebug->m_material->SetIntUniform("_TrailNum", m_TrailNum);
			m_TrailDebug->m_material->SetIntUniform("_LineSegment", m_LineSegment);
			}, GL_POINTS, true, m_LineSegment * m_TrailNum);
#endif // _DEBUG
	}
}