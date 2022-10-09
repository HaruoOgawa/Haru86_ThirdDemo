#pragma once
#include <memory>

class MeshRendererComponent;

namespace app
{
	class MoonSea
	{
		std::shared_ptr<MeshRendererComponent> m_MoonSeaMeshRenderer;

		bool								   m_IsLeaveEarth;
	private:
	public:
		MoonSea();
		virtual ~MoonSea() = default;

		void Update();
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}