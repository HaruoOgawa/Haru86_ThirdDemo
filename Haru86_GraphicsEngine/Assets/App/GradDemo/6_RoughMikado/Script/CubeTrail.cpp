#include "CubeTrail.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "Assets/App/GradDemo/CommonScript/TrailRenderer.h"

namespace app
{
	CubeTrail::CubeTrail():
		m_InstancedCube(nullptr),
		m_TrailRenderer(nullptr),
		m_CubeNum(2048)
	{
		m_TrailRenderer = std::make_shared<TrailRenderer>(0, 0, 1, 4, 32, 4);
		m_InstancedCube = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::CUBE,
			RenderingSurfaceType::RASTERIZER,
			"#version 430\n"
			"#define SegmentBufferBinding " + std::to_string(m_TrailRenderer->GetBufferIndexTrailSegment()) + "\n" +
			std::string(
				#include "../Shader/CubeTrail.vert"
			),
			shaderlib::Standard_frag
		);
	}

	void CubeTrail::Update(float time)
	{
		m_TrailRenderer->Update(time);
	}

	void CubeTrail::Draw()
	{
		//m_TrailRenderer->Draw();
		
		m_InstancedCube->Draw([&]() {
			m_InstancedCube->m_material->SetIntUniform("_TrailNum", m_TrailRenderer->GetTrailNum());
			m_InstancedCube->m_material->SetIntUniform("_LineSegment", m_TrailRenderer->GetLineSegment());
			m_InstancedCube->m_material->SetFloatUniform("_Scale", 0.5f);
			m_InstancedCube->m_material->SetFloatUniform("_rOffRange", 2.5f);
		}, GL_TRIANGLES, true, m_CubeNum);
	}
}