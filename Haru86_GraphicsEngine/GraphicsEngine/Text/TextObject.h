#pragma once
#include <string>
#include <glm/glm.hpp>

namespace text {
	static class TextObject
	{
	public:
		static void Draw(const std::string& Text, float FontSize = 0.1f, float Width = 1.0f, float Height = 1.0f,
			const glm::vec3& Pos = glm::vec3(0.0f), const glm::vec4& Color = glm::vec4(1.0f), bool IsTextAlignLeft = false, bool IsUseBack = false);
	};
}