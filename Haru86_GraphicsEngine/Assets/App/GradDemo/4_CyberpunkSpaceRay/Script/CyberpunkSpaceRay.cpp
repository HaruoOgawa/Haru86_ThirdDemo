#include "CyberpunkSpaceRay.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

namespace app {
	CyberpunkSpaceRay::CyberpunkSpaceRay() :
		m_MeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
#include "../Shader/CyberpunkSpaceRay.frag"
			)
			);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = true;
	}

	void CyberpunkSpaceRay::Update(float time)
	{

	}

	void CyberpunkSpaceRay::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MeshRenderer->Draw();
		}
	}

	void CyberpunkSpaceRay::UpdateTimeLine(float time)
	{
		if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 4)
		{
			if (time >= 133.5f)
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					float Rate = glm::clamp((time - 133.5f) * 2.0f, 0.0f, 1.0f);

					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseGridWave", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWaveCustom", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WavePower", Rate);
					
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteNoise", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteNoisePower", Rate * 2.0f);
				};
			}
			
		}
	}
}