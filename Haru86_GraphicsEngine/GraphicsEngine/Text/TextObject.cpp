#include "TextObject.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "TextRenderer.h"

namespace text {
	void TextObject::Draw(std::string DrawTexLine)
	{
		GraphicsRenderer::GetInstance()->m_TextRenderer->Draw(DrawTexLine);
	}
}