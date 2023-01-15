#include "CShaderEditor.h"
#include "CShaderCode.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG


namespace editor
{
	CShaderEditor::CShaderEditor():
		m_FontSize(0.01f),
		m_CurerentShadeIndex(0),
		m_MaxDrawCount(75)
	{
		m_DrawLineIndexList.push_back(0); // シェーダーが切り変わる度にクリアして0をセットする
	}

	CShaderEditor::~CShaderEditor()
	{
		m_ShaderNameList.clear();
		m_ShaderCodeList.clear();
	}

	void CShaderEditor::AddShaderMap(const std::string& ShaderName, const std::string& ShaderCode)
	{
		m_ShaderNameList.push_back(ShaderName);
		m_ShaderCodeList.push_back(std::make_shared<CShaderCode>(ShaderCode));
	}

	void CShaderEditor::Update()
	{
		//
		int DrawLineSize = static_cast<int>(m_DrawLineIndexList.size());
		int CodeLineSize = static_cast<int>(m_ShaderCodeList[m_CurerentShadeIndex]->GetCodeLineList().size());
		float xOffset = 0.05f, yAdjust = 1.25f;

		// インデックスの更新処理
		if (DrawLineSize < m_MaxDrawCount) // 描画リストのサイズがまだ最大数以下の場合、単純に少しずつ増やす 
		{
			m_DrawLineIndexList.push_back(m_DrawLineIndexList.back() + 1);
		}
		// 描画の最大数を越したのでインデックスをスライドさせる
		else if(m_DrawLineIndexList.back() < CodeLineSize)
		{
			m_DrawLineIndexList.erase(m_DrawLineIndexList.begin());
			m_DrawLineIndexList.shrink_to_fit();

			m_DrawLineIndexList.push_back(m_DrawLineIndexList.back() + 1);
		}

		// スクロール処理
		for (const auto& DrawLineIndex : m_DrawLineIndexList)
		{
			if (DrawLineIndex >= CodeLineSize) break;
			auto& TextLine = m_ShaderCodeList[m_CurerentShadeIndex]->GetCodeLineList()[DrawLineIndex];
			TextLine.m_Pos.y += m_FontSize * 2.0f * yAdjust;
		}
	}

	void CShaderEditor::Draw()
	{
		//
		int CodeLineSize = static_cast<int>(m_ShaderCodeList[m_CurerentShadeIndex]->GetCodeLineList().size());
		float xOffset = 0.05f, yAdjust = 1.25f;

		// 描画
		for (const auto& DrawLineIndex : m_DrawLineIndexList)
		{
			if (DrawLineIndex >= CodeLineSize) break;

			const auto& TextLine = m_ShaderCodeList[m_CurerentShadeIndex]->GetCodeLineList()[DrawLineIndex];
			text::TextObject::Draw(TextLine.m_TextLine, m_FontSize, 1.35f, 3.0f, TextLine.m_Pos + glm::vec3(xOffset, 0.0f, 0.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, true);
		}
	}
}