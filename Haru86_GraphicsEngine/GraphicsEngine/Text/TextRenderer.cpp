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
			shaderlib::Text_vert,
			shaderlib::Text_frag
		);

		m_TextMeshRenderer->useAlphaTest = true;
	}
	
	TextRenderer::~TextRenderer()
	{
	}

	void TextRenderer::Draw(const std::string& Text, float FontSize, float WRange, float HRange, const glm::vec3& Pos, const glm::vec4& Color)
	{
		const char* TextData = Text.data();
		std::vector<std::vector<char>> StructuredText;
		int PushIndex = 0;
		StructuredText.push_back(std::vector<char>());
		for (float n = 0.0f; n < static_cast<float>(Text.size()); n++)
		{
			const auto PushText = TextData[static_cast<size_t>(n)];
			if (PushText == '\n') // â¸çsÇ»ÇÃÇ≈çsÇéüÇ…êiÇﬂÇÈ
			{
				if(n < static_cast<float>(Text.size()) - 1.0f) StructuredText.push_back(std::vector<char>());
				PushIndex++;
			}
			else
			{
				StructuredText[PushIndex].push_back(PushText);
			}
		}
		//Console::Log("_____________________________________\n");
		const float NumOfRowChar = static_cast<float>(StructuredText.size());
		for (float row = 0.0f; row < NumOfRowChar; row++)
		{
			const auto& Arr = StructuredText[row];
			const float NumOfColChar = static_cast<float>(Arr.size());

			for (float col = 0.0f; col < static_cast<float>(Arr.size()); col++)
			{
				//
				const auto& CharTex = GraphicsMain::GetInstance()->m_TTFFactory->GetFTChar(static_cast<unsigned char>(Arr[static_cast<size_t>(col)]));
				const float PixelSize = static_cast<float>(GraphicsMain::GetInstance()->m_TTFFactory->GetPixelSize());
				const float Left = static_cast<float>(CharTex->GetLeft()) / PixelSize;
				const float Top = static_cast<float>(CharTex->GetTop()) / PixelSize;
				const float Width = static_cast<float>(CharTex->GetWidth()) / PixelSize;
				const float Height = static_cast<float>(CharTex->GetHeight()) / PixelSize;

				//Console::Log("Char: %c / PixelSize: %f / Left: %d / Top: %d / Width: %d / Height: %d\n", Arr[static_cast<size_t>(col)], PixelSize, CharTex->GetLeft(), CharTex->GetTop(), CharTex->GetWidth(), CharTex->GetHeight());

				//
				glm::vec3 scale = glm::vec3(Width, Height, 0.0f) * FontSize;
				m_TextMeshRenderer->m_transform->m_scale = scale;

				//
				float XVal = (col - NumOfColChar * 0.5f) * FontSize * WRange;
				float YVal = (row - NumOfRowChar * 0.5f) * FontSize * HRange * (-1.0f);
				float LVal = (Left * scale.x)			 ;
				float TVal = (Top * scale.y)			 ;

				m_TextMeshRenderer->m_transform->m_position = glm::vec3(XVal + LVal, YVal + TVal, 0.0f) + Pos;

				//
				m_TextMeshRenderer->Draw([&]() {
					m_TextMeshRenderer->m_material->SetVec4Uniform("_Color", Color);
					if (CharTex)
					{
						CharTex->SetActive(GL_TEXTURE0);
						m_TextMeshRenderer->m_material->SetIntUniform("_MainTex", 0);
					}
					}, GL_TRIANGLES, false, 0);
				if (CharTex)CharTex->SetEnactive(GL_TEXTURE0);
			}
		}
	}
}