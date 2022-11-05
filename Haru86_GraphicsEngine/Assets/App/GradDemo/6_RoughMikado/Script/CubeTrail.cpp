#include "CubeTrail.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/Material.h"

namespace roughmikado
{
	CubeTrail::CubeTrail():
		m_InstancedCube(nullptr),
		m_TrailBaseBuffer(nullptr),
		m_BaseTrailCS(nullptr),
		m_LineSegment(16),
		m_CubeNum(1024),
#ifdef _DEBUG
		m_TrailDebug(std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::POINT,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::Standard_vert,
			shaderlib::Standard_frag,
			std::string(
				#include "../Shader/TrailDebug.geom"
			)
		)),
#endif // _DEBUG

		m_BufferIndexTrailBase(0)
	{
		// •Ï”‚Ì‰Šú‰»
		m_InstancedCube = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::CUBE,
			RenderingSurfaceType::RASTERIZER,
			std::string(
				#include "../Shader/CubeTrail.vert"
			),
			shaderlib::Standard_frag
		);

		m_TrailBaseBuffer = std::make_shared<ComputeBuffer>(sizeof(STrs) * m_LineSegment);
		
		m_BaseTrailCS = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER,"","","","","",
			std::string(
				#include "../Shader/CubeTrail.comp"
			)
		);

		// GPGPU‚Ì€”õ
		InitBuffer();

		// ‰‰ñGPGPU‚ÌÀs
		Start();
	}

	CubeTrail::~CubeTrail()
	{
		if (m_TrailBaseBuffer)
		{
			m_TrailBaseBuffer->DeleteBuffer();
			m_TrailBaseBuffer = nullptr;
		}
	}

	void CubeTrail::InitBuffer()
	{
		{
			std::vector<STrs> InitData;
			for (int i = 0; i < m_LineSegment; i++) { InitData.push_back(STrs(i)); }
			m_TrailBaseBuffer->SetData<std::vector<STrs>>(InitData);
			m_BaseTrailCS->SetBufferToCS(m_TrailBaseBuffer, m_BufferIndexTrailBase);
			m_InstancedCube->m_material->SetBufferToMat(m_TrailBaseBuffer, m_BufferIndexTrailBase);

#ifdef _DEBUG
			m_TrailDebug->m_material->SetBufferToMat(m_TrailBaseBuffer, m_BufferIndexTrailBase);
#endif // _DEBUG
		}
	}

	void CubeTrail::Start()
	{
		{
			m_BaseTrailCS->SetActive();
			m_BaseTrailCS->Dispatch(1, 1, 1);
		}
	}

	void CubeTrail::Update(float time)
	{
		{
			m_BaseTrailCS->SetActive();
			m_BaseTrailCS->SetIntUniform("_KernelIndex", 0);
			m_BaseTrailCS->Dispatch(1, 1, 1);
		}
	}

	void CubeTrail::Draw()
	{
		//m_InstancedCube->Draw([]() {}, GL_TRIANGLES, true, 16);

#ifdef _DEBUG
		m_TrailDebug->Draw([]() {},GL_POINTS, true, m_LineSegment);
#endif // _DEBUG

	}
}