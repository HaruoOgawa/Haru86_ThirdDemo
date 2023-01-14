#include "CShaderEditor.h"
#include "CShaderCode.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG


namespace editor
{
	CShaderEditor::CShaderEditor()
	{

	}

	CShaderEditor::~CShaderEditor()
	{

	}

	void CShaderEditor::AddShaderMap(const std::string& ShaderName, const std::string& ShaderCode)
	{
		m_ShaderNameList.push_back(ShaderName);
		m_ShaderCodeList.push_back(std::make_shared<CShaderCode>(ShaderCode));
	}

	void CShaderEditor::Update()
	{

	}

	void CShaderEditor::Draw()
	{
		const float num = static_cast<float>(m_ShaderCodeList[0]->GetCodeLineList().size());
		int useIndex = static_cast<int>(glm::mod(GraphicsMain::GetInstance()->m_SecondsTime, num));

		const auto& ShaderName = m_ShaderNameList[0];
		const auto& TextLine = m_ShaderCodeList[0]->GetCodeLineList()[useIndex];

		//Console::Log("ShaderName: %s / TextLine: %s\n", ShaderName.c_str(), TextLine.c_str());

		text::TextObject::Draw(TextLine, 0.05f, 1.35f, 3.0f, glm::vec3(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, true);
	}
}