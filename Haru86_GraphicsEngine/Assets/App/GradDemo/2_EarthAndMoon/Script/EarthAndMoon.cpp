#include "EarthAndMoon.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace app {
	EarthAndMoon::EarthAndMoon():
		m_MeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/EarthAndMoon.frag"
			)
		);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = true;
	}

	void EarthAndMoon::Update()
	{

	}

	void EarthAndMoon::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MeshRenderer->Draw();
		}
	}

	void EarthAndMoon::UpdateTimeLine(float time)
	{

	}
}