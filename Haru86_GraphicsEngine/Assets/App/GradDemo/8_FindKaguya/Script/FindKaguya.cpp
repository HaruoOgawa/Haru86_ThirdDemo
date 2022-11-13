#include "FindKaguya.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"
#include "../../6_RoughMikado/Script/CubeTrail.h"
#include "GraphicsEngine/Graphics/RenderBuffer.h"

namespace app
{
	FindKaguya::FindKaguya():
		m_Voxel(nullptr),
		m_RayTrail(nullptr)
		//m_CubeTrail(nullptr)
	{
		m_Voxel = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			shaderdshader::Voxel_frag
		);

		//m_CubeTrail = std::make_shared<CubeTrail>(0, 4, 5, 32, 4, 32, 4, 0.1f);

		m_RenderBufferList.push_back(std::make_shared<graphic::RenderBuffer>(
			static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion),
			static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion),
			GL_RGBA16F, GL_RGBA, GL_FLOAT
		));
		
		m_RenderBufferList.push_back(std::make_shared<graphic::RenderBuffer>(
			static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion),
			static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion),
			GL_RGBA16F, GL_RGBA, GL_FLOAT
		));

		m_RayTrail = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../../CommonShader/RayTrail.frag"
			)
		);

		m_RayTrail->useAlphaTest = true;

		m_RenderBufferList[0]->Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), []() {}, true);
		m_RenderBufferList[1]->Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), []() {}, true);
	}

	void FindKaguya::Update(float time)
	{
		//m_CubeTrail->Update(time);
	}

	void FindKaguya::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::COLOR)
			{
				m_RenderBufferList[0]->Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), [&]() {
					m_RayTrail->Draw([&]() {
						m_RenderBufferList[1]->GetFrameTexture()->SetActive(GL_TEXTURE0);
						m_RayTrail->m_material->SetTexUniform("_BufferA", 0);
						});
					m_RenderBufferList[1]->GetFrameTexture()->SetEnactive(GL_TEXTURE0);
					}, true);
				
				m_RenderBufferList[1]->Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), [&]() {
					m_RayTrail->Draw([&]() {
						m_RenderBufferList[0]->GetFrameTexture()->SetActive(GL_TEXTURE0);
						m_RayTrail->m_material->SetTexUniform("_BufferA", 0);
						});
					m_RenderBufferList[0]->GetFrameTexture()->SetEnactive(GL_TEXTURE0);
					}, true);
			}

			m_Voxel->Draw([&]() {
				m_Voxel->m_material->SetIntUniform("_MapIndex", 0);
				m_Voxel->m_material->SetIntUniform("_UseTex", 1);

				m_RenderBufferList[0]->GetFrameTexture()->SetActive(GL_TEXTURE0);
				m_Voxel->m_material->SetTexUniform("_BufferA", 0);
			});
			m_RenderBufferList[0]->GetFrameTexture()->SetEnactive(GL_TEXTURE0);
		}
		else
		{
			//m_CubeTrail->Draw();
		}
	}

	void FindKaguya::UpdateTimeLine(float time)
	{
	}
}