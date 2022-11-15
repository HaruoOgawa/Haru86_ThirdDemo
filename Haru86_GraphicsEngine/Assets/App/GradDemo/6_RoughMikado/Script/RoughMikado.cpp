#include "RoughMikado.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../SharedShader.h"
#include "CubeTrail.h"
#include "AuraTrail.h"
#include "AuraCore.h"
#include "BoidsGPGPU.h"
#include "EnergyField.h"

namespace app
{
	RoughMikado::RoughMikado():
		m_TrailCube(nullptr),
		m_AuraTrail(nullptr),
		m_AuraCore(nullptr),
		m_AuraBackRenderer(nullptr),
		m_BoidsGPGPU(nullptr),
		m_EnergyField(nullptr)
	{
		m_EnergyField = std::make_shared<EnergyField>();

		//m_BoidsGPGPU = std::make_shared<BoidsGPGPU>(16, 16, 4, 5);

		//m_TrailCube = std::make_shared<CubeTrail>(2, 0, 1, 16, 32, 16, 32, 0.1f, m_BoidsGPGPU);
		//m_AuraTrail = std::make_shared<AuraTrail>();
		//m_AuraCore = std::make_shared<AuraCore>();
		/*m_AuraBackRenderer = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert,
			std::string(
				#include "../Shader/AuraBackground.frag"
			)
		);*/
	}

	void RoughMikado::Update(float time)
	{
		m_EnergyField->Update();

		//GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(1.0f));

		//m_TrailCube->Update(time);
		//m_AuraTrail->Update(time);
		//m_BoidsGPGPU->Update(time);
	}

	void RoughMikado::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_EnergyField->Draw(IsRaymarching);
			//m_AuraBackRenderer->Draw();
		}
		else
		{
			//m_TrailCube->Draw();
			//m_AuraTrail->Draw();
			//m_AuraCore->Draw();
		}
	}

	void RoughMikado::UpdateTimeLine(float time)
	{

	}
}