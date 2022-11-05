#include "RoughMikado.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"

namespace app
{
	RoughMikado::RoughMikado():
		m_TrailAura(nullptr),
		m_TrailCube(nullptr)
	{

	}

	void RoughMikado::Update(float time)
	{

	}

	void RoughMikado::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{

		}
	}

	void RoughMikado::UpdateTimeLine(float time)
	{

	}
}