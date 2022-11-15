#include "ChangeOfMind.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/Primitive.h"

namespace app
{
	ChangeOfMind::ChangeOfMind() :
		m_NegativeSphereMeshRenderer(nullptr),
		m_NegativeSphereCoreMeshRenderer(nullptr),
		m_Voxel(nullptr),
		m_Triangle(nullptr)
	{
		// m_NegativeSphereMeshRenderer
		{
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			Primitive::CreateSphere(&VertexData, &Dimention, &Indices, 32.0f, 32.0f, 1.0f);

			m_NegativeSphereMeshRenderer = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				RenderingSurfaceType::RASTERIZER,
				VertexData, Dimention, Indices,
				shaderlib::Standard_vert,
				std::string(
					#include "../Shader/NegativeSphere.frag"
				),
				std::string(
					#include "../Shader/NegativeSphere.geom"
				)
			);

			m_NegativeSphereMeshRenderer->useZTest = true;
			m_NegativeSphereMeshRenderer->useAlphaTest = true;
			m_NegativeSphereMeshRenderer->IsMulMatOnVert = false;
		}

		// m_NegativeSphereCoreMeshRenderer
		{
			m_NegativeSphereCoreMeshRenderer = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				PrimitiveType::SPHERE,
				RenderingSurfaceType::RASTERIZER,
				std::string(
					#include "../Shader/NegativeSphereCore.vert"
				),
				shaderlib::Standard_frag
			);

			m_NegativeSphereCoreMeshRenderer->m_transform->m_scale = glm::vec3(0.75f);
			m_NegativeSphereCoreMeshRenderer->useDoubleSlided = false;
		}

		// m_Voxel
		{
			m_Voxel = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				PrimitiveType::BOARD,
				RenderingSurfaceType::RAYMARCHING,
				shaderlib::StandardRenderBoard_vert,
				std::string(
					#include "../../8_FindKaguya/Shader/Voxel.frag"
				)
			);

			m_Voxel->useAlphaTest = true;
		}
		
		// m_Triangle
		{
			m_Triangle = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				PrimitiveType::BOARD,
				RenderingSurfaceType::RAYMARCHING,
				shaderlib::StandardRenderBoard_vert,
				std::string(
					#include "../Shader/Triangle.frag"
				)
			);

			m_Triangle->useAlphaTest = true;
			m_Triangle->useZTest = false;
		}
	}

	void ChangeOfMind::Update(float time)
	{

	}

	void ChangeOfMind::Draw(bool IsRaymarching)
	{
		bool DebugDraw = (glm::mod(GraphicsMain::GetInstance()->m_SecondsTime, 2.0f) < 1.0);
		DebugDraw = true;
		//DebugDraw = false;
		float Alpha = abs(sin(GraphicsMain::GetInstance()->m_SecondsTime));

		if (IsRaymarching)
		{
			//if (DebugDraw)
			{
				m_Voxel->Draw([=]() {
					m_Voxel->m_material->SetIntUniform("_MapIndex", 1);
					m_Voxel->m_material->SetFloatUniform("_Alpha", Alpha);
				});
			}
			//else
			{
				m_Triangle->Draw([=]() {
					m_Triangle->m_material->SetFloatUniform("_Alpha",1.0f - Alpha);
				});
			}
		}
		else
		{
			

			if (Alpha > 0.5f)
			{
				m_NegativeSphereMeshRenderer->Draw([=]() {
					m_NegativeSphereMeshRenderer->m_material->SetFloatUniform("_Alpha", Alpha);
				}, GL_POINTS);
				m_NegativeSphereCoreMeshRenderer->Draw([&]() {
					m_NegativeSphereCoreMeshRenderer->m_material->SetIntUniform("_UseRim", 1);
					m_NegativeSphereCoreMeshRenderer->m_material->SetVec4Uniform("_RimColor", glm::vec4(0.25f, 0.0f, 0.0f, Alpha));
					m_NegativeSphereCoreMeshRenderer->m_material->SetFloatUniform("_RimPower", 1.75f);
					m_NegativeSphereCoreMeshRenderer->m_material->SetFloatUniform("_RimMulVal", 1.5f);
					});
			}
		}
	}

	void ChangeOfMind::UpdateTimeLine(float time)
	{

	}

}