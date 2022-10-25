#pragma once
#include <memory>

class MeshRendererComponent;
namespace app
{
	class ChangeOfMind
	{
		std::shared_ptr<MeshRendererComponent> m_NegativeSphereMeshRenderer;
		std::shared_ptr<MeshRendererComponent> m_NegativeSphereCoreMeshRenderer;

		std::shared_ptr<MeshRendererComponent> m_PositiveSphereFrame;
		std::shared_ptr<MeshRendererComponent> m_PositiveSphereCore;
		std::shared_ptr<MeshRendererComponent> m_PositiveSphereDecolate;
	public:
		ChangeOfMind();
		virtual ~ChangeOfMind() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}