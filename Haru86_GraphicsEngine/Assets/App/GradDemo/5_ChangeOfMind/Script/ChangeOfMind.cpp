#include "ChangeOfMind.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

namespace app
{
	ChangeOfMind::ChangeOfMind() :
		m_MeshRenderer(nullptr),
		m_NegativeSphereMeshRenderer(nullptr),
		m_PositiveSphereMeshRenderer(nullptr)
	{
		m_NegativeSphereMeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::SPHERE,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag,
			std::string(
				#include "../Shader/NegativeSphere.geom"
			)
		);

		m_NegativeSphereMeshRenderer->useZTest = true;
		m_NegativeSphereMeshRenderer->useAlphaTest = true;
		m_NegativeSphereMeshRenderer->IsMulMatOnVert = false;
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
			m_NegativeSphereMeshRenderer->Draw([]() {},GL_TRIANGLES);
		}
	}

	void ChangeOfMind::UpdateTimeLine(float time)
	{

	}

}