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
		m_PositiveSphereFrame(nullptr),
		m_PositiveSphereCore(nullptr)
	{
		// m_NegativeSphereMeshRenderer
		{
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			Primitive::CreateSphere(&VertexData, &Dimention, &Indices, 32.0f, 32.0f, 1.0f);

			m_NegativeSphereMeshRenderer = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				RenderingSurfaceType::RASTERIZER,
				VertexData, Dimention, Indices,
				shaderlib::ShaderLib::Standard_vert,
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
				shaderlib::ShaderLib::Standard_vert,
				shaderlib::ShaderLib::Standard_frag
			);

			m_NegativeSphereCoreMeshRenderer->m_transform->m_scale = glm::vec3(0.5f);
		}

		// m_PositiveSphereFrame
		{
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			Primitive::CreateSphere(&VertexData, &Dimention, &Indices, 6.0f, 6.0f, 1.0f);

			m_PositiveSphereFrame = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				PrimitiveType::Icosahedron,
				RenderingSurfaceType::RASTERIZER,
				shaderlib::ShaderLib::Standard_vert,
				shaderlib::ShaderLib::Standard_frag
			);
		}

		// m_PositiveSphereCore
		{
			m_PositiveSphereCore = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				PrimitiveType::SPHERE,
				RenderingSurfaceType::RASTERIZER,
				shaderlib::ShaderLib::Standard_vert,
				shaderlib::ShaderLib::Standard_frag
			);
		}
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
			/*m_NegativeSphereMeshRenderer->Draw([]() {},GL_POINTS);
			m_NegativeSphereCoreMeshRenderer->Draw();*/

			// スフィアーモデルをIssueのリンクをもとに改善する
			m_PositiveSphereFrame->Draw([]() {},GL_LINES);
			//m_PositiveSphereCore->Draw();
		}
	}

	void ChangeOfMind::UpdateTimeLine(float time)
	{

	}

}