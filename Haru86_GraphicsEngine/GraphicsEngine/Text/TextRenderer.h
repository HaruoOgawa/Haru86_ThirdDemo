#pragma once
#include <memory>
#include <string>

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

		void Draw(std::string DrawTextLine);
	};
}

