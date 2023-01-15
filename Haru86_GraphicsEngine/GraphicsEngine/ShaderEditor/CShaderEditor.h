#pragma once
#include <vector>
#include <memory>
#include <string>

namespace editor
{
	class CShaderCode;
	struct CShaderCodeLine;

	class CShaderEditor
	{
		std::vector<std::string> m_ShaderNameList;
		std::vector<std::shared_ptr<CShaderCode>> m_ShaderCodeList;

		float m_FontSize;

		int m_CurerentShadeIndex;
		int m_MaxDrawCount;
		std::vector<int> m_DrawLineIndexList;
	public:
		CShaderEditor();
		virtual ~CShaderEditor();

		void AddShaderMap(const std::string& ShaderName, const std::string& ShaderCode);

		void Update();
		void Draw();
	};
}
