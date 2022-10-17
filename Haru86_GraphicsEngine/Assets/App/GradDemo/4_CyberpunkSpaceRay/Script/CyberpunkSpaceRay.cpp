#include "CyberpunkSpaceRay.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace app {
	CyberpunkSpaceRay::CyberpunkSpaceRay() :
		m_MeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::StandardRenderBoard_vert,
			std::string(
#include "../Shader/CyberpunkSpaceRay.frag"
			)
			);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = true;
	}

	void CyberpunkSpaceRay::Update()
	{

	}

	void CyberpunkSpaceRay::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MeshRenderer->Draw();
		}
	}

	void CyberpunkSpaceRay::UpdateTimeLine(float time)
	{

	}
}