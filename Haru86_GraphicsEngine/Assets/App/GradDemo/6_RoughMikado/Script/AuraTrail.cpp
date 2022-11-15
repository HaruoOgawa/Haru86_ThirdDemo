#include "AuraTrail.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "Assets/App/GradDemo/CommonScript/TrailRenderer.h"

namespace app
{
	AuraTrail::AuraTrail() :
		m_InstancedAura(nullptr),
		m_TrailRenderer(nullptr),
		m_AuraNum(4096)
	{
		//m_TrailRenderer = std::make_shared<TrailRenderer>(1, 2, 3, 16, 6, 16, 6, 0.1f/*0.01f*/);
		m_InstancedAura = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			//PrimitiveType::POINT,
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER,
			"#version 430\n"
			"#define SegmentBufferBinding " + std::to_string(m_TrailRenderer->GetBufferIndexTrailSegment()) + "\n" +
			std::string(
				#include "../Shader/CubeTrail.vert"
			),
			std::string(
				#include "../Shader/AuraTrail.frag"
			)/*,
			"#version 430\n"
			"#define SegmentBufferBinding " + std::to_string(m_TrailRenderer->GetBufferIndexTrailSegment()) + "\n" +
			std::string(
				#include "../Shader/AuraTrail.geom"
			)*/
		);
	}

	void AuraTrail::Update(float time)
	{
		m_TrailRenderer->Update(time);
	}

	void AuraTrail::Draw()
	{
		//m_TrailRenderer->Draw();

		m_InstancedAura->Draw([&]() {
			m_InstancedAura->m_material->SetIntUniform("_TrailNum", m_TrailRenderer->GetTrailNum());
			m_InstancedAura->m_material->SetIntUniform("_LineSegment", m_TrailRenderer->GetLineSegment());
			m_InstancedAura->m_material->SetIntUniform("_UseToTanScale", 1);
			m_InstancedAura->m_material->SetFloatUniform("_Scale", 0.1f);
			m_InstancedAura->m_material->SetFloatUniform("_rOffRange", 0.0f);
			m_InstancedAura->m_material->SetFloatUniform("_Radius", 4.0f);
			}, GL_TRIANGLES, true, m_AuraNum);
	}
}