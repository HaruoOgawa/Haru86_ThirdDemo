#include "RoughMikado.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"
#include "CubeTrail.h"
#include "AuraTrail.h"

namespace app
{
	RoughMikado::RoughMikado():
		m_TrailCube(nullptr),
		m_AuraTrail(nullptr),
		m_AuraBackRenderer(nullptr)
	{
		m_TrailCube = std::make_shared<CubeTrail>();
		m_AuraTrail = std::make_shared<AuraTrail>();
		m_AuraBackRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/AuraBackground.frag"
			)
		);
	}

	void RoughMikado::Update(float time)
	{
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(1.0f));

		m_TrailCube->Update(time);
		m_AuraTrail->Update(time);
	}

	void RoughMikado::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_AuraBackRenderer->Draw();
		}
		else
		{
			m_TrailCube->Draw();
			//m_AuraTrail->Draw();
		}
	}

	void RoughMikado::UpdateTimeLine(float time)
	{

	}
}