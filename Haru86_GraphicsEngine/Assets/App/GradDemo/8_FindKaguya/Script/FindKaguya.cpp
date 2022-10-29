#include "FindKaguya.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"

namespace app
{
	FindKaguya::FindKaguya():
		m_Voxel(nullptr)
	{
		m_Voxel = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			shaderdshader::Voxel_frag
		);
	}

	void FindKaguya::Update(float time)
	{
	}

	void FindKaguya::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_Voxel->Draw();
		}
	}

	void FindKaguya::UpdateTimeLine(float time)
	{
	}
}