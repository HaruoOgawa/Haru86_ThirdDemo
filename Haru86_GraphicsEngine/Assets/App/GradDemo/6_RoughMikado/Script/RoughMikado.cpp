#include "RoughMikado.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"
#include "CubeTrail.h"

namespace app
{
	RoughMikado::RoughMikado():
		m_TrailAura(nullptr),
		m_TrailCube(nullptr)
	{
		m_TrailCube = std::make_shared<CubeTrail>();
	}

	void RoughMikado::Update(float time)
	{
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(1.0f));

		m_TrailCube->Update(time);
	}

	void RoughMikado::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{

		}
		else
		{
			m_TrailCube->Draw();
		}
	}

	void RoughMikado::UpdateTimeLine(float time)
	{

	}
}