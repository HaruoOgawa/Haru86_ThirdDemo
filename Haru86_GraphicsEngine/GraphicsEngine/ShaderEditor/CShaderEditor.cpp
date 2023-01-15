#include "CShaderEditor.h"
#include "CShaderCode.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace editor
{
	CShaderEditor::CShaderEditor():
		m_FontSize(0.01f),
		m_CurerentShadeIndex(0),
		m_MaxDrawCount(75),
		m_EditorRenderer(nullptr),
		m_EditorOffset(0.0f)
	{
		m_EditorRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,RenderingSurfaceType::RASTERIZER,
			shaderlib::Standard_vert, shaderlib::Standard_frag
		);

		m_EditorRenderer->useZTest = false;
		m_EditorRenderer->useAlphaTest = true;
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

	// リアルタイムでテキストラインを描画するのは流石に重すぎるのでプリレンダしておく
	void CShaderEditor::CreatePreCodeLineTexture()
	{
		//
		for (auto& ShaderCode : m_ShaderCodeList)
		{
			// 変数準備
			int CodeLineSize = static_cast<int>(ShaderCode->GetCodeLineList().size());
			float xOffset = 0.05f, yAdjust = 1.25f;
			std::vector<int> DrawLineIndexList;
			unsigned int FrameIndex = 0;

			m_FrameTextureList.push_back(std::make_shared<Texture>());
			DrawLineIndexList.push_back(0);

			// フレームバッファを生成
			const auto& ScreenSize = GraphicsRenderer::GetInstance()->GetScreenSize();
			glm::ivec2 BufferSize = glm::ivec2(static_cast<int>(ScreenSize.x), static_cast<int>(ScreenSize.y) * CodeLineSize / m_MaxDrawCount);
			m_BufferYRepeatNum.push_back(CodeLineSize / m_MaxDrawCount);

			GraphicsRenderer::GetInstance()->CreateFrameBuffer(BufferSize.x, BufferSize.y, m_FrameTextureList.back(), FrameIndex, GL_RGBA, GL_RGBA);
		
			// 描画する文字列の位置を決定
			while (DrawLineIndexList.back() < CodeLineSize)
			{
				DrawLineIndexList.push_back(DrawLineIndexList.back() + 1);

				// スクロール処理
				for (const auto& DrawLineIndex : DrawLineIndexList)
				{
					if (DrawLineIndex >= CodeLineSize) break;
					auto& TextLine = ShaderCode->GetCodeLineList()[(CodeLineSize - 1) - DrawLineIndex];
					TextLine.m_Pos.y -= m_FontSize * 2.0f * yAdjust;
				}
			}

			// フレームバッファに描画する
			//
			GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
			glBindFramebuffer(GL_FRAMEBUFFER, FrameIndex);
			GraphicsMain::GetInstance()->m_TargetFrameIndex = FrameIndex;
			glViewport(0, 0, BufferSize.x, BufferSize.y);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 描画
			for (const auto& DrawLineIndex : DrawLineIndexList)
			{
				if (DrawLineIndex >= CodeLineSize) break;

				const auto& TextLine = ShaderCode->GetCodeLineList()[DrawLineIndex];
				text::TextObject::Draw(TextLine.m_TextLine, m_FontSize, 1.35f, 3.0f, TextLine.m_Pos + glm::vec3(xOffset, 0.0f, 0.0f),
					glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), true, true);
			}
		}

		// ShaderCodeはもう使わないので解放する
		m_ShaderCodeList.clear();
	}

	void CShaderEditor::Update()
	{
		//
		m_CurerentShadeIndex = GraphicsMain::GetInstance()->GetAppSceneIndex();

		//
		float time = GraphicsMain::GetInstance()->m_SecondsTime, sTime = GraphicsMain::GetInstance()->GetSceneStartTime(), eTime = GraphicsMain::GetInstance()->GetSceneEndTime();
		float SceneProgress = glm::clamp((time - sTime) / (eTime - sTime), 0.0f, 1.0f);

		m_EditorOffset = -1.0f * SceneProgress * m_BufferYRepeatNum[m_CurerentShadeIndex];
	}

	void CShaderEditor::Draw()
	{
		if (m_EditorRenderer)
		{
			m_EditorRenderer->Draw([&]() {
				m_EditorRenderer->m_material->SetVec4Uniform("_Color", glm::vec4(1.0f));
				m_EditorRenderer->m_material->SetIntUniform("_IsMOnly", 1);
				m_EditorRenderer->m_material->SetIntUniform("_IsMulMatOnVert", 0);
				m_EditorRenderer->m_material->SetIntUniform("_UseLighting", 0);
				m_EditorRenderer->m_material->SetIntUniform("_UseMainEditorTex", 1);
				m_FrameTextureList[m_CurerentShadeIndex]->SetActive(GL_TEXTURE0);
				m_EditorRenderer->m_material->SetIntUniform("_MainTex", 0);
				m_EditorRenderer->m_material->SetFloatUniform("_EditorOffset", m_EditorOffset);
			}, GL_TRIANGLES, false, 0);
			m_FrameTextureList[m_CurerentShadeIndex]->SetEnactive(GL_TEXTURE0);
		}
	}
}