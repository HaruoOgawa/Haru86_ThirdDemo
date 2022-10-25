#pragma once
#include <string>

namespace shaderlib {
	class ShaderLib { // ビルトインシェーダーをShaderLibで定義する(カスタムシェーダはApp::Startから直接渡す)
	public:
		static const std::string Standard_vert;
		static const std::string Standard_frag;
		static const std::string DepthColor_frag;
		static const std::string Line2Cylinder_geom;
		static const std::string GridPlane_frag;
		static const std::string StandardRenderBoard_vert;
		static const std::string StandardRenderBoard_frag;
		static const std::string LatePostProcess_frag;
		static const std::string PolygonPostProcess_frag;
		static const std::string PolygonRaymarchingMixer_vert;
		static const std::string PolygonRaymarchingMixer_frag;
		static const std::string Text_frag;
	};
}