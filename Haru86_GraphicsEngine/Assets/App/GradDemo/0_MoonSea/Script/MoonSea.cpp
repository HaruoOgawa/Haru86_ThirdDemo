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
		m_IsLeaveEarth(false),
		m_UseTextIndex(-1),
		m_Alpha(0.0f),
		m_DrawRaySpaceShip(false)
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

			if (m_DrawRaySpaceShip) m_RaySpaceShip->Draw([&](){
				m_RaySpaceShip->m_material->SetIntUniform("_IsUseShowing", 1);
				m_RaySpaceShip->m_material->SetFloatUniform("_ShowingFinTime", 30.0f);
				m_RaySpaceShip->m_material->SetFloatUniform("_ShowDuration", 14.0f);
				m_RaySpaceShip->m_material->SetFloatUniform("_MoveStartTime", 31.0f);
				m_RaySpaceShip->m_material->SetFloatUniform("_MoveTimeDuration", 11.0f);

				m_RaySpaceShip->m_material->SetIntUniform("_TRSIndex", 0);
				m_RaySpaceShip->m_material->SetIntUniform("_RefMapIndex", 0);
			});
		}
		else
		{
			if(m_UseTextIndex == 0) text::TextObject::Draw("DemoScene 64k Intro", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f), glm::vec4(1.0f, 1.0f, 1.0f,m_Alpha));
			else if (m_UseTextIndex == 1) text::TextObject::Draw("The Tale of the Bamboo-Cutter", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, m_Alpha));
			else if (m_UseTextIndex == 2) text::TextObject::Draw("CG Engineering\nHaru86_", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, m_Alpha));
			else if (m_UseTextIndex == 3) text::TextObject::Draw("Music melcom", 0.05f, 1.0f, 3.0f, glm::vec3(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, m_Alpha));
		}
	}

	void MoonSea::UpdateTimeLine(float time)
	{
		unsigned int SceneIndex = GraphicsMain::GetInstance()->GetAppSceneIndex();
		if (SceneIndex == 0)
		{
			m_IsLeaveEarth = false;

			//m_Alpha = 1.0f;
			float pi = 3.1415f;
			if (time >= 1.0 && time < 4.5f) { m_UseTextIndex = 0; m_Alpha = sin(glm::clamp((time>=4.0)? 4.5f - time : time - 1.0f, 0.0f, 1.0f) * pi * 0.5f); }
			else if (time >= 4.5f && time < 8.2f) { m_UseTextIndex = 1; m_Alpha = sin(glm::clamp((time>= 7.2f)? 8.2f - time : time - 4.5f, 0.0f, 1.0f) * pi * 0.5f); }
			else if (time >= 8.2f && time < 12.3f) { m_UseTextIndex = 2; m_Alpha = sin(glm::clamp((time>= 11.3f)? 12.3f - time : time - 8.2f, 0.0f, 1.0f) * pi * 0.5f); }
			else if (time >= 12.3f && time < 15.5f) { m_UseTextIndex = 3; m_Alpha = sin(glm::clamp((time>= 14.5f)? 15.5f - time : time - 12.3f, 0.0f, 1.0f) * pi * 0.5f); }
			else if (time >= 16.0f) { 
				m_DrawRaySpaceShip = true;
				GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
					glm::cos(time * 0.25f) * 3.0f,
					0.0f,
					glm::sin(time * 0.25f) * 3.0f
				);

			}
			else { m_UseTextIndex = -1; m_Alpha = 0.0f; }
			
		}
		else if (SceneIndex == 1)
		{
			m_IsLeaveEarth = true;

			{
				float h = time - 44.0f;
				glm::vec3 ro = glm::vec3(0.0f, h, 1.5f), ta = glm::vec3(0.0f, h, 0.0f);
				int CameraID = int(glm::floor(glm::mod(time, 3.0f))); // 0,1,2

				if (CameraID == 0)
				{
					// down side camera
					ta += 2.0f;
					ro = glm::vec3(cos(time), ro.y, sin(time));
				}
				else if (CameraID == 1)
				{
					ro += glm::vec3(
						0.0f,
						0.0f,
						1.5f
					);
				}
				else if (CameraID == 2)
				{
					// upside camera
					ro += glm::vec3(2.0);
				}

				GraphicsMain::GetInstance()->m_MainCamera->m_position = ro;
				GraphicsMain::GetInstance()->m_MainCamera->m_center = ta;
			}

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