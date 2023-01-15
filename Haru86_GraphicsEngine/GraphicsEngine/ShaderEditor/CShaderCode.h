#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace editor
{
	struct CShaderCodeLine
	{
		std::string m_TextLine;
		glm::vec3   m_Pos;
	};

	class CShaderCode
	{
		std::vector<CShaderCodeLine> m_CodeLineList;
	public:
		CShaderCode(const std::string& Code);
		std::vector<CShaderCodeLine>& GetCodeLineList() { return m_CodeLineList; }
	};
}