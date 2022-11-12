#include "AuraCore.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

namespace app
{
	AuraCore::AuraCore():
		m_CoreRenderer(nullptr)
	{
		m_CoreRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::Standard_vert,
			std::string(
				#include "../Shader/AuraTrail.frag"
			)
		);

		m_CoreRenderer->m_transform->m_scale = glm::vec3(2.0f);
	}

	void AuraCore::Update()
	{
	}

	void AuraCore::Draw()
	{
		float BillBoardNum = 4.0f;
		for (float n = 0.0f; n < BillBoardNum; n++)
		{
			float a = 2.0f * 3.1415f / BillBoardNum;
			m_CoreRenderer->m_transform->m_rotation = glm::vec3(0.0f, n * a, 0.0f);
			m_CoreRenderer->Draw([&]() {
				m_CoreRenderer->m_material->SetIntUniform("_UseColor", 1);
				m_CoreRenderer->m_material->SetVec3Uniform("_Color", glm::vec3(1.0f, 0.8f, 0.6f));
			});
		}
	}
}