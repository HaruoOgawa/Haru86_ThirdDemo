#pragma once
#include <string>

namespace shaderlib {
	const std::string Standard_vert = {
		#include "GraphicsEngine/Graphics/Shader/standard.vert"
	};

	const std::string Standard_frag = {
		#include "GraphicsEngine/Graphics/Shader/standard.frag"
	};

	const std::string StandardRenderBoard_vert = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.vert"
	};

	const std::string LatePostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/LatePostProcess.frag"
	};

	const std::string Voxel_frag = {
		#include "Assets/App/GradDemo/8_FindKaguya/Shader/Voxel.frag"
	};
}