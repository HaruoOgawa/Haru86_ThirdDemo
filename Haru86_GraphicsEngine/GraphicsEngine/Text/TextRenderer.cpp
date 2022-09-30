#include "TextRenderer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "TTFFactory.h"

namespace text
{
	TextRenderer::TextRenderer()
	{
		m_TextMeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Text_frag
		);
	}
	
	TextRenderer::~TextRenderer()
	{
	}

	void TextRenderer::Draw(std::string DrawTextLine)
	{
		const char* TextData = DrawTextLine.data();
		
		for (float n = 0.0f; n < static_cast<float>(DrawTextLine.size()); n++)
		{
			//
			float XVal = (n / (static_cast<float>(DrawTextLine.size()) - 1)) * 2.0f - 1.0f;
			XVal *= 1.5f;

			//
			m_TextMeshRenderer->m_transform->m_scale = glm::vec3(0.5f);
			m_TextMeshRenderer->m_transform->m_position = glm::vec3(XVal, 0.0f, 0.0f);
			const auto& CharTex = GraphicsMain::GetInstance()->m_TTFFactory->GetFTChar(static_cast<unsigned char>(TextData[static_cast<size_t>(n)]));
			m_TextMeshRenderer->Draw(GL_TRIANGLES, false, 0, [&]() {
				if (CharTex)
				{
					CharTex->SetActive(GL_TEXTURE1);
					m_TextMeshRenderer->m_material->SetIntUniform("_MainTex", 1);
				}
				});
			if (CharTex)CharTex->SetEnactive(GL_TEXTURE1);
		}
	}
}