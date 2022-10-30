#pragma once
#include <memory>

class MeshRendererComponent;
namespace app
{
	class CyberpunkSpaceRay
	{
		std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
	public:
		CyberpunkSpaceRay();
		virtual ~CyberpunkSpaceRay() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}

