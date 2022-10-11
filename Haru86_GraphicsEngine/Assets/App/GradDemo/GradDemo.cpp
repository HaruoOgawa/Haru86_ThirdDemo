#include "GradDemo.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "0_MoonSea/Script/MoonSea.h"
#include "2_EarthAndMoon/Script/EarthAndMoon.h"
#include "3_MoonTravel/Script/MoonTravel.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG


namespace app
{
    GradDemo::GradDemo() :
        m_SceneIndex(0),
        m_LocalTime(0.0f),
        m_MoonSea(nullptr),
        m_EarthAndMoon(nullptr),
        m_MoonTravel(nullptr)
    {
    }

    void GradDemo::Start()
    {
#ifdef _DEBUG
        // 時間のオフセット
        GraphicsMain::GetInstance()->m_SecondsTimeOffset = 30.0f;// シーンを飛ばすためのオフセット

        // デバッグ用
        /*{
            m_DebugTimeLock = true;
            if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
        }*/
#endif

        m_MoonSea = std::make_shared<app::MoonSea>();
        m_EarthAndMoon = std::make_shared<app::EarthAndMoon>();
        m_MoonTravel = std::make_shared<app::MoonTravel>();
    }

    void GradDemo::Load()
    {

    }

    void GradDemo::Update()
    {
        if(m_LocalTime)

        m_MoonSea->Update();
        m_EarthAndMoon->Update();
        m_MoonTravel->Update();
    }

    void GradDemo::Draw(bool IsRaymarching)
    {
        if (m_SceneIndex == 0 || m_SceneIndex == 1) m_MoonSea->Draw(IsRaymarching);
        if (m_SceneIndex == 2) m_EarthAndMoon->Draw(IsRaymarching);
        if (m_SceneIndex == 3) m_MoonTravel->Draw(IsRaymarching);

        //text::TextObject::Draw("Haru86_");
    }

    void GradDemo::UpdateTimeline()
    {
        m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTime;

        // とりあえずデバッグで3つ分繰り替えす
        //m_LocalTime = glm::mod(m_LocalTime, 20.0f);
        //if (m_LocalTime >= 0.0f && m_LocalTime < 5.0f) // MoonSea
        //{
        //    m_SceneIndex = 0;
        //}
        //else if (m_LocalTime >= 5.0f && m_LocalTime < 10.0f) // LeaveEarth
        //{
        //    m_SceneIndex = 1;
        //}
        //else if (m_LocalTime >= 10.0f && m_LocalTime < 15.0f) // EarthAndMoon
        //{
        //    m_SceneIndex = 2;
        //}
        //else if (m_LocalTime >= 15.0f && m_LocalTime < 20.0f) // MoonTravel
        //{
        //    m_SceneIndex = 3;
        //}

#ifdef _DEBUG
        m_SceneIndex = 0;

        Console::Log("m_LocalTime: %f\n", m_LocalTime);
#endif // _DEBUG

        m_MoonSea->UpdateTimeLine(m_LocalTime);
        m_EarthAndMoon->UpdateTimeLine(m_LocalTime);
        m_MoonTravel->UpdateTimeLine(m_LocalTime);
    }
}