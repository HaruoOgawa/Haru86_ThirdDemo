#include "TextObject.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "TextRenderer.h"

namespace text {
	void TextObject::Draw(const std::string& Text, float FontSize, float WAdjust, float HAdjust, const glm::vec3& Pos, const glm::vec4& Color,
		bool IsTextAlignLeft, bool IsUseBack)
	{
		GraphicsRenderer::GetInstance()->m_TextRenderer->Draw(Text, FontSize, WAdjust, HAdjust, Pos, Color, IsTextAlignLeft, IsUseBack);
	}
}