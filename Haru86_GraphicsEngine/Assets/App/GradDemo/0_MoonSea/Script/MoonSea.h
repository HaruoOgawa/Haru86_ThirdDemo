#pragma once
#include <memory>

class MeshRendererComponent;

namespace app
{
	class MoonSea
	{
		std::shared_ptr<MeshRendererComponent> m_MoonSeaMeshRenderer;
		std::shared_ptr<MeshRendererComponent> m_RaySpaceShip;

		bool								   m_IsLeaveEarth;

		//
		int									   m_UseTextIndex;
		float								   m_Alpha;
	private:
	public:
		MoonSea();
		virtual ~MoonSea() = default;

		void Update(float time);
		void Draw(bool IsRaymarching);
		void UpdateTimeLine(float time);
	};
}