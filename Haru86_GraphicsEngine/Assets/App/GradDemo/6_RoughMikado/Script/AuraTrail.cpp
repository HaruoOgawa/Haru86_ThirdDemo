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
		m_TrailRenderer = std::make_shared<TrailRenderer>(1, 2, 3, 4, 8, 4);
		m_InstancedAura = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER,
			"#version 430\n"
			"#define SegmentBufferBinding " + std::to_string(m_TrailRenderer->GetBufferIndexTrailSegment()) + "\n" +
			std::string(
				#include "../Shader/CubeTrail.vert"
			),std::string(
				#include "../Shader/AuraTrail.frag"
			)
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
			m_InstancedAura->m_material->SetIntUniform("_TrailNum", 16);
			m_InstancedAura->m_material->SetIntUniform("_LineSegment", 16);
			m_InstancedAura->m_material->SetFloatUniform("_Scale", 0.5f);
			m_InstancedAura->m_material->SetFloatUniform("_rOffRange", 0.5f);
			}, GL_TRIANGLES, true, m_AuraNum);
	}
}