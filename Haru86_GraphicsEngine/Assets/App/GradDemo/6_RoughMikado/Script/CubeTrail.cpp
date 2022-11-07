#include "CubeTrail.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "Assets/App/GradDemo/CommonScript/TrailRenderer.h"

namespace app
{
	CubeTrail::CubeTrail():
		m_InstancedCube(nullptr),
		m_TrailRenderer(nullptr),
		m_CubeNum(1024)
	{
		m_TrailRenderer = std::make_shared<TrailRenderer>(
			std::string(
#include "../Shader/TrailSegment.comp"
			), 0, 1, 16, 16, 16
		);

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
		m_TrailRenderer->Draw();
		//m_InstancedCube->Draw([]() {}, GL_TRIANGLES, true, m_CubeNum);
	}
}