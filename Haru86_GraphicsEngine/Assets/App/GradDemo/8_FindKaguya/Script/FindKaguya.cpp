#include "FindKaguya.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"
#include "../../6_RoughMikado/Script/CubeTrail.h"

namespace app
{
	FindKaguya::FindKaguya():
		m_Voxel(nullptr),
		m_CubeTrail(nullptr)
	{
		m_Voxel = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			shaderdshader::Voxel_frag
		);

		m_CubeTrail = std::make_shared<CubeTrail>(0, 4, 5, 32, 4, 32, 4, 0.1f);
	}

	void FindKaguya::Update(float time)
	{
		m_CubeTrail->Update(time);
	}

	void FindKaguya::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_Voxel->Draw([&]() {
				m_Voxel->m_material->SetIntUniform("_MapIndex", 0);
			});
		}
		else
		{
			m_CubeTrail->Draw();
		}
	}

	void FindKaguya::UpdateTimeLine(float time)
	{
	}
}