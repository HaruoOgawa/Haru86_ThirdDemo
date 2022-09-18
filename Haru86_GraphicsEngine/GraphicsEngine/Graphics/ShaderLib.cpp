#include "ShaderLib.h"
#include <iostream>
namespace shaderlib { // プリプロセッサでどれをリンクするか選ぶのもありかも？？ => 例えばBloomを使わないのにBloomShaderをリンクするのは無駄
	const std::string ShaderLib::Standard_vert = {
		#include "GraphicsEngine/Graphics/Shader/standard.vert"
	};
	
	const std::string ShaderLib::Standard_frag = {
		#include "GraphicsEngine/Graphics/Shader/standard.frag"
	};
	
	const std::string ShaderLib::DepthColor_frag = {
		#include "GraphicsEngine/Graphics/Shader/DepthColor.frag"
	};
	
#ifdef _DEBUG
	const std::string ShaderLib::GridPlane_frag = {
		#include "GraphicsEngine/Graphics/Shader/GridPlane.frag"
};
#endif // _DEBUG

	const std::string ShaderLib::StandardRenderBoard_vert = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.vert"
	};
	
	const std::string ShaderLib::StandardRenderBoard_frag = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.frag"
	};

	const std::string ShaderLib::LatePostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/LatePostProcess.frag"
	};
	
	const std::string ShaderLib::PolygonPostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/PolygonPostProcess.frag"
	};
	
	const std::string ShaderLib::PolygonRaymarchingMixer_vert = {
		#include "GraphicsEngine/Graphics/Shader/PolygonRaymarchingMixer.vert"
	};
	
	const std::string ShaderLib::PolygonRaymarchingMixer_frag = {
		#include "GraphicsEngine/Graphics/Shader/PolygonRaymarchingMixer.frag"
	};
}