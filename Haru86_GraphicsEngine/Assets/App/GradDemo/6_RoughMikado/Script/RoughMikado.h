#pragma once
#include <memory>

class MeshRendererComponent;

namespace app
{
	class CubeTrail;
	class AuraTrail;
	class AuraCore;
	class BoidsGPGPU;
	class EnergyField;

	class RoughMikado
	{
		std::shared_ptr<CubeTrail> m_TrailCube;
		std::shared_ptr<AuraTrail> m_AuraTrail;
		std::shared_ptr<AuraCore> m_AuraCore;
		std::shared_ptr<MeshRendererComponent> m_AuraBackRenderer;

		std::shared_ptr<BoidsGPGPU> m_BoidsGPGPU;

		std::shared_ptr<EnergyField> m_EnergyField;
	public:
		RoughMikado();
		~RoughMikado() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}
