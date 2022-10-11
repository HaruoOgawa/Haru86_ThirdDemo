#include "EarthAndMoon.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

namespace app {
	EarthAndMoon::EarthAndMoon():
		m_MeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/EarthAndMoon.frag"
			)
		);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = true;
	}

	void EarthAndMoon::Update()
	{

	}

	void EarthAndMoon::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MeshRenderer->Draw();
		}
	}

	void EarthAndMoon::UpdateTimeLine(float time)
	{
		unsigned int SceneIndex = GraphicsMain::GetInstance()->GetAppSceneIndex();

		if (SceneIndex == 2)
		{
			if (time < 62.0f)
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhite", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteVal", 62.0f - time);
				};
			}
			else
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhite", 0);
				};
			}

			m_MeshRenderer->m_calllback = [=]() {
				m_MeshRenderer->m_material->SetFloatUniform("_MoveVal", (time - 61.0f)/13.0f);
			};
		}
	}
}