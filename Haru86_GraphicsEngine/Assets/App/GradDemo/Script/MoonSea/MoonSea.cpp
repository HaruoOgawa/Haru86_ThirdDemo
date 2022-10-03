#include "MoonSea.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace app
{
	MoonSea::MoonSea():
		m_MoonSeaMeshRenderer(nullptr)
	{
		m_MoonSeaMeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::StandardRenderBoard_vert,
			std::string(
				#include "../../Shader/MoonSea/MoonSea.frag"
			)
		);
		m_MoonSeaMeshRenderer->useZTest = false;
		m_MoonSeaMeshRenderer->useAlphaTest = true;
	}

	void MoonSea::Update()
	{
	}

	void MoonSea::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MoonSeaMeshRenderer->Draw();
		}
	}

	void MoonSea::UpdateTimeLine(float time)
	{
	}
}