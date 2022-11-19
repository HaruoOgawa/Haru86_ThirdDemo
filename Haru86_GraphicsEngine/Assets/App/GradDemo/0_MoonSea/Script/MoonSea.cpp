#include "MoonSea.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "GraphicsEngine/Text/TextObject.h"

namespace app
{
	MoonSea::MoonSea():
		m_MoonSeaMeshRenderer(nullptr),
		m_RaySpaceShip(nullptr),
		m_IsLeaveEarth(false)
	{
		m_MoonSeaMeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/MoonSea.frag"
			)
		);
		m_MoonSeaMeshRenderer->useZTest = false;
		m_MoonSeaMeshRenderer->useAlphaTest = true;
		
		m_RaySpaceShip = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/RaySpaceShip.frag"
			)
		);
		m_RaySpaceShip->useZTest = false;
		m_RaySpaceShip->useAlphaTest = true;
	}

	void MoonSea::Update(float time)
	{
	}

	void MoonSea::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MoonSeaMeshRenderer->Draw([this]() {
				m_MoonSeaMeshRenderer->m_material->SetFloatUniform("_LeaveStartTime", 44.0f);
			});

			//m_RaySpaceShip->Draw();
		}
		else
		{
			text::TextObject::Draw("DemoScene 64k Intro", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f));
			//text::TextObject::Draw("The Tale of the Bamboo-Cutter", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f));
			//text::TextObject::Draw("CG Engineering\n Haru86_", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f));
		}
	}

	void MoonSea::UpdateTimeLine(float time)
	{
		unsigned int SceneIndex = GraphicsMain::GetInstance()->GetAppSceneIndex();
		if (SceneIndex == 0)
		{
			m_IsLeaveEarth = false;

			
		}
		else if (SceneIndex == 1)
		{
			m_IsLeaveEarth = true;

			if (time >= 60.0f)
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhite", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteVal", time - 60.0f);
				};
			}
		}
	}
}