#pragma once
#include <memory>
class MeshRendererComponent;

namespace app
{
	class AuraCore
	{
		std::shared_ptr<MeshRendererComponent> m_CoreRenderer;
	public:
		AuraCore();
		virtual ~AuraCore() = default;

		void Update();
		void Draw();
	};
}