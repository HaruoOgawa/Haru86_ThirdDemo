#pragma once
#include <string>

namespace shaderlib { // プリプロセッサでどれをリンクするか選ぶのもありかも？？ => 例えばBloomを使わないのにBloomShaderをリンクするのは無駄
	const std::string Standard_vert = {
		#include "GraphicsEngine/Graphics/Shader/standard.vert"
	};

	const std::string Standard_frag = {
		#include "GraphicsEngine/Graphics/Shader/standard.frag"
	};

	const std::string DepthColor_frag = {
		#include "GraphicsEngine/Graphics/Shader/DepthColor.frag"
	};

	const std::string StandardRenderBoard_vert = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.vert"
	};

	const std::string StandardRenderBoard_frag = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.frag"
	};

	const std::string LatePostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/LatePostProcess.frag"
	};

	const std::string PolygonRaymarchingMixer_frag = {
		#include "GraphicsEngine/Graphics/Shader/PolygonRaymarchingMixer.frag"
	};

	const std::string Text_vert = {
		#include "GraphicsEngine/Graphics/Shader/Text.vert"
	};
	
	const std::string Text_frag = {
		#include "GraphicsEngine/Graphics/Shader/Text.frag"
	};
}