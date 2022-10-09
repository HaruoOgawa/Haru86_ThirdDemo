#include "GradDemo.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "1_MoonSea/Script/MoonSea.h"
#include "3_EarthAndMoon/Script/EarthAndMoon.h"
#include "4_MoonTravel/Script/MoonTravel.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG


namespace app
{
    GradDemo::GradDemo() :
        m_SceneIndex(0),
        m_LocalTime(0.0f),
        m_TestRenderer(nullptr),
        m_MoonSea(nullptr),
        m_EarthAndMoon(nullptr),
        m_MoonTravel(nullptr)
    {
    }

    void GradDemo::Start()
    {
#ifdef _DEBUG
        // 時間のオフセット
        GraphicsMain::GetInstance()->m_SecondsTimeOffset = 0.0f;// シーンを飛ばすためのオフセット

        // デバッグ用
        /*{
            m_DebugTimeLock = true;
            if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
        }*/
#endif

        m_TestRenderer = std::make_shared<MeshRendererComponent>(
            std::make_shared<TransformComponent>(),
            PrimitiveType::SPHERE,
            RenderingSurfaceType::RASTERIZER,
            shaderlib::ShaderLib::Standard_vert,
            shaderlib::ShaderLib::Standard_frag
        );

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
        //m_TestRenderer->Draw();
        //text::TextObject::Draw("Haru86_");
        
        if (m_SceneIndex == 0) m_MoonSea->Draw(IsRaymarching);
        if (m_SceneIndex == 1) m_EarthAndMoon->Draw(IsRaymarching);
        if (m_SceneIndex == 2) m_MoonTravel->Draw(IsRaymarching);
    }

    void GradDemo::UpdateTimeline()
    {
        m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTime;

        // とりあえずデバッグで3つ分繰り替えす
        m_LocalTime = glm::mod(m_LocalTime, 15.0f);

        if (m_LocalTime >= 0.0f && m_LocalTime < 5.0f)
        {
            m_SceneIndex = 0;
        }
        else if (m_LocalTime >= 5.0f && m_LocalTime < 10.0f)
        {
            m_SceneIndex = 1;
        }
        else if (m_LocalTime >= 10.0f && m_LocalTime < 15.0f)
        {
            m_SceneIndex = 2;
        }

#ifdef _DEBUG
        Console::Log("m_LocalTime: %f\n", m_LocalTime);
#endif // _DEBUG

        m_MoonSea->UpdateTimeLine(m_LocalTime);
        m_EarthAndMoon->UpdateTimeLine(m_LocalTime);
        m_MoonTravel->UpdateTimeLine(m_LocalTime);
    }
}