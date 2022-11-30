#include "MoonTravel.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace app {
	MoonTravel::MoonTravel():
		m_MeshRenderer(nullptr),
		m_RaySpaceShip(nullptr)
	{
		m_MeshRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/MoonTravel.frag"
			)
		);

		m_MeshRenderer->useZTest = false;
		m_MeshRenderer->useAlphaTest = true;

		m_RaySpaceShip = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../../0_MoonSea/Shader/RaySpaceShip.frag"
			)
		);
		m_RaySpaceShip->useZTest = false;
		m_RaySpaceShip->useAlphaTest = true;
	}

	void MoonTravel::Update(float time)
	{

	}

	void MoonTravel::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MeshRenderer->Draw();

			m_RaySpaceShip->Draw([&]() {
				m_RaySpaceShip->m_material->SetIntUniform("_TRSIndex", -1);
				m_RaySpaceShip->m_material->SetIntUniform("_RefMapIndex", 1);
				m_RaySpaceShip->m_material->SetFloatUniform("_CorrectionValue", 0.1f);
			});
		}
	}

	void MoonTravel::UpdateTimeLine(float time)
	{
		if(time >= 74.0f && time < 88.0f){}
		else if(time >= 88.0f && time < 96.0f){}
		else if(time >= 96.0f && time < 103.0f){}
	}
}