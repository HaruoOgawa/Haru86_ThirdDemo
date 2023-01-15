#include "CShaderCode.h"
#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace editor
{
	CShaderCode::CShaderCode(const std::string& Code)
	{
		// 言語処理(演出用なのである程度の誤差は許容範囲)
		for (int i = 0, startIndex = 0, indent = 0; i < Code.length(); i++)
		{
			const auto& InputChar = Code[i];
			const auto& InputNextChar = Code[i + 1];
			if (
				InputChar == '\n' ||
				InputChar == ';' ||
				InputChar == '{' ||
				(InputChar == '}') ||
				(InputChar == ')' && InputNextChar != ';' && InputNextChar != ',' && InputNextChar != ')' && InputNextChar != '*' && InputNextChar != '+' && InputNextChar != '-' && InputNextChar != '/')
				)
			{
				std::string result;
				result.resize(i - startIndex + 1);
				std::memcpy(&result[0], &Code[startIndex], result.size());

				// インデントを減らす
				if (InputChar == '}') indent--;

				// インデントの追加
				for (int n = 0; n < indent; n++)
				{
					result = "	" + result;
				}

				// 結果の描画
#ifdef _DEBUG
				//Console::Log("%s\n", result.c_str());
#endif // _DEBUG

				//
				m_CodeLineList.push_back(CShaderCodeLine{ result, glm::vec3(0.0f, -1.0f, 0.0f) });

				// インデントを増やす
				if (InputChar == '{') indent++;

				startIndex = i + 1;
			}
		}
	}
}