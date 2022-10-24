#pragma once
#include <memory>

class MeshRendererComponent;
namespace app
{
	class ChangeOfMind
	{
		std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
		std::shared_ptr<MeshRendererComponent> m_NegativeSphereMeshRenderer;
		std::shared_ptr<MeshRendererComponent> m_NegativeSphereCoreMeshRenderer;
		std::shared_ptr<MeshRendererComponent> m_PositiveSphereMeshRenderer;
	public:
		ChangeOfMind();
		virtual ~ChangeOfMind() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}