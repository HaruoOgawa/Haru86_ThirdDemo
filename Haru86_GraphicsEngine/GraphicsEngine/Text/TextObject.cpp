#include "TextObject.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "TextRenderer.h"

namespace text {
	void TextObject::Draw(const std::string& Text, float FontSize, float Width, float Height, const glm::vec3& Pos, const glm::vec4& Color)
	{
		GraphicsRenderer::GetInstance()->m_TextRenderer->Draw(Text, FontSize, Width, Height, Pos, Color);
	}
}