#include "FindKaguya.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

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
			std::string(
				#include "../Shader/Voxel.frag"
			)
		);
	}

	void FindKaguya::Update(float time)
	{
	}

	void FindKaguya::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_Voxel->Draw([&]() {
				m_Voxel->m_material->SetIntUniform("_MapIndex", 0);
				m_Voxel->m_material->SetIntUniform("_UseTex", 1);
			});
		}
	}

	void FindKaguya::UpdateTimeLine(float time)
	{
	}
}