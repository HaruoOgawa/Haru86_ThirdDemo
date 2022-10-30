#pragma once
#include <string>

namespace shaderdshader
{
	const std::string Voxel_frag = std::string(
		#include "SharedShader/Voxel.frag"
	);
}