#include "GradDemo.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "0_MoonSea/Script/MoonSea.h"
#include "2_EarthAndMoon/Script/EarthAndMoon.h"
#include "3_MoonTravel/Script/MoonTravel.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"

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
        GraphicsMain::GetInstance()->m_SecondsTimeOffset = 40.0f;// シーンを飛ばすためのオフセット

        // 音楽のミュート
        GraphicsMain::GetInstance()->m_SoundPlayer->Mute(true);

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

        if (m_LocalTime >= 0.0f && m_LocalTime < 44.0f)
        {
            m_SceneIndex = 0;
        }
        else if (m_LocalTime >= 44.0f && m_LocalTime < 61.0f)
        {
            m_SceneIndex = 1;
        }
        else if (m_LocalTime >= 61.0f && m_LocalTime < 74.0f) // 13s
        {
            m_SceneIndex = 2;
        }
        else if (m_LocalTime >= 74.0f && m_LocalTime < 103.0f)
        {
            m_SceneIndex = 3;
        }
        else if (m_LocalTime >= 103.0f && m_LocalTime < 118.0f)
        {
            m_SceneIndex = 4;
        }
        else if (m_LocalTime >= 118.0f && m_LocalTime < 134.0f)
        {
            m_SceneIndex = 4;
        }
        /*else if (m_LocalTime >= 134.0f && m_LocalTime < 154.0f)
        {
            m_SceneIndex = 5;
        }
        else if (m_LocalTime >= 134.0f && m_LocalTime < 154.0f)
        {
            m_SceneIndex = 6;
        }
        else if (m_LocalTime >= 134.0f && m_LocalTime < 154.0f)
        {
            m_SceneIndex = 7;
        }
        else if (m_LocalTime >= 134.0f && m_LocalTime < 271.0f)
        {
            m_SceneIndex = 8;
        }*/
        else if (m_LocalTime >= 271.0f && m_LocalTime < 286.0f)
        {
            m_SceneIndex = 9;
        }

#ifdef _DEBUG

        Console::Log("m_LocalTime: %f\n", m_LocalTime);
#endif // _DEBUG

        m_MoonSea->UpdateTimeLine(m_LocalTime);
        m_EarthAndMoon->UpdateTimeLine(m_LocalTime);
        m_MoonTravel->UpdateTimeLine(m_LocalTime);
    }
}