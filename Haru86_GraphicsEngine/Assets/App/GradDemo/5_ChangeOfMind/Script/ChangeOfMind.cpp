#include "ChangeOfMind.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

namespace app
{
	ChangeOfMind::ChangeOfMind() :
		m_MeshRenderer(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::SPHERE,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
		);

		m_MeshRenderer->useZTest = true;
		m_MeshRenderer->useAlphaTest = true;
	}

	void ChangeOfMind::Update(float time)
	{

	}

	void ChangeOfMind::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			
		}
		else
		{
			m_MeshRenderer->Draw();
		}
	}

	void ChangeOfMind::UpdateTimeLine(float time)
	{

	}

}