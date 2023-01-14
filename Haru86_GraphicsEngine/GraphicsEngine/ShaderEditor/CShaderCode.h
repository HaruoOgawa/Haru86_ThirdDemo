#pragma once
#include <string>
#include <vector>

namespace editor
{
	class CShaderCode
	{
		std::vector<std::string> m_CodeLineList;
	public:
		CShaderCode(const std::string& Code);
		const std::vector<std::string>& GetCodeLineList() const { return m_CodeLineList; }
	};
}