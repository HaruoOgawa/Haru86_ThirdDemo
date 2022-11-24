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

	const std::string Line2Cylinder_geom = {
		#include "GraphicsEngine/Graphics/Shader/Line2Cylinder.geom"
	};

#ifdef _DEBUG
	const std::string GridPlane_frag = {
		#include "GraphicsEngine/Graphics/Shader/GridPlane.frag"
	};
#endif // _DEBUG

	const std::string StandardRenderBoard_vert = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.vert"
	};

	const std::string StandardRenderBoard_frag = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.frag"
	};

	const std::string LatePostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/LatePostProcess.frag"
	};

	const std::string PolygonPostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/PolygonPostProcess.frag"
	};

	const std::string PolygonRaymarchingMixer_vert = {
		#include "GraphicsEngine/Graphics/Shader/PolygonRaymarchingMixer.vert"
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