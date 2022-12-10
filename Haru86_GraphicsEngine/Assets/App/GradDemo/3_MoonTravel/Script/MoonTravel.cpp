#include "MoonTravel.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

namespace app {
	MoonTravel::MoonTravel():
		m_MeshRenderer(nullptr),
		m_RaySpaceShip(nullptr),
		m_CyberMeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/MoonTravel.frag"
			)
		);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = true;

		m_RaySpaceShip = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../../0_MoonSea/Shader/RaySpaceShip.frag"
			)
		);
		m_RaySpaceShip->useZTest = false;
		m_RaySpaceShip->useAlphaTest = true;

		m_CyberMeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/CyberpunkSpaceRay.frag"
			)
		);

		m_CyberMeshRenderer->useZTest = false;
		m_CyberMeshRenderer->useAlphaTest = true;
	}

	void MoonTravel::Update(float time)
	{

	}

	void MoonTravel::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			float time = GraphicsMain::GetInstance()->m_SecondsTime;
			float Alpha = glm::clamp(time - 117.5f, 0.0f, 1.0f);

			m_MeshRenderer->Draw([&]() {
				m_MeshRenderer->m_material->SetFloatUniform("_ShowStartTime", 103.0f);
				m_MeshRenderer->m_material->SetFloatUniform("_ShowTimeDuration", 5.0f);
				m_MeshRenderer->m_material->SetFloatUniform("_Alpha",1.0f - Alpha);
			});

			if (time >= 117.5f)
			{
				m_CyberMeshRenderer->Draw([&]() {
					m_CyberMeshRenderer->m_material->SetFloatUniform("_Alpha", Alpha);
					});
			}

			m_RaySpaceShip->Draw([&]() {
				m_RaySpaceShip->m_material->SetIntUniform("_TRSIndex", -1);
				m_RaySpaceShip->m_material->SetIntUniform("_RefMapIndex", 1);
				m_RaySpaceShip->m_material->SetFloatUniform("_CorrectionValue", 0.1f);
			});
		}
	}

	void MoonTravel::UpdateTimeLine(float time)
	{
		if(time >= 74.0f && time < 88.0f){}
		else if(time >= 88.0f && time < 96.0f){}
		else if(time >= 96.0f && time < 103.0f){}

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