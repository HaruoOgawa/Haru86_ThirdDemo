#pragma once
#include <vector>
#include <memory>
#include <string>

namespace editor
{
	class CShaderCode;

	class CShaderEditor
	{
		std::vector<std::string> m_ShaderNameList;
		std::vector<std::shared_ptr<CShaderCode>> m_ShaderCodeList;
	public:
		CShaderEditor();
		virtual ~CShaderEditor();

		void AddShaderMap(const std::string& ShaderName, const std::string& ShaderCode);

		void Update();
		void Draw();
	};
}
