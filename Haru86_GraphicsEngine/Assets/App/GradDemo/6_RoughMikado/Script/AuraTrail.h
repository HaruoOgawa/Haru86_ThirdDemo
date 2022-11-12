#pragma once
#include <memory>
class MeshRendererComponent;

namespace app
{
	class TrailRenderer;

	class AuraTrail
	{
		std::shared_ptr<MeshRendererComponent> m_InstancedAura;
		std::shared_ptr<TrailRenderer> m_TrailRenderer;
		int m_AuraNum;
	public:
		AuraTrail();
		virtual ~AuraTrail() = default;

		void Update(float time);
		void Draw();
	};
}