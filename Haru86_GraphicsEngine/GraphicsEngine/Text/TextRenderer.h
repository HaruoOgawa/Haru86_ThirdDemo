#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>

class MeshRendererComponent;

namespace text
{
	class TextRenderer
	{
		std::shared_ptr<MeshRendererComponent> m_TextMeshRenderer;
	private:
	public:
		TextRenderer();
		virtual ~TextRenderer();

		void Draw(const std::string& Text, float FontSize, float Width, float Height, const glm::vec3& Pos, const glm::vec4& Color);
	};
}

