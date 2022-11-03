#include "MountFuji.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace app {
	MountFuji::MountFuji() :
		m_MeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/MountFuji.frag"
			)
		);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = false;
	}

	void MountFuji::Update(float time)
	{

	}

	void MountFuji::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MeshRenderer->Draw();
		}
	}

	void MountFuji::UpdateTimeLine(float time)
	{

	}
}