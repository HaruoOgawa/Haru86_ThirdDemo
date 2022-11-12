#pragma once
#include <memory>

class MeshRendererComponent;

namespace app {
	class FeatherRobe
	{
		std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
	public:
		FeatherRobe();
		virtual ~FeatherRobe() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}
