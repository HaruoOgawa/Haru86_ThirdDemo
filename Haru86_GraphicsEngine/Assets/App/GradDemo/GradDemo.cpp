#include "GradDemo.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "0_MoonSea/Script/MoonSea.h"
#include "2_EarthAndMoon/Script/EarthAndMoon.h"
#include "3_MoonTravel/Script/MoonTravel.h"
#include "4_CyberpunkSpaceRay/Script/CyberpunkSpaceRay.h"
#include "5_ChangeOfMind/Script/ChangeOfMind.h"
#include "6_RoughMikado/Script/RoughMikado.h"
#include "8_FindKaguya/Script/FindKaguya.h"
#include "9_MountFuji/Script/MountFuji.h"
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
        m_MoonTravel(nullptr),
        m_CyberpunkSpaceRay(nullptr),
        m_ChangeOfMind(nullptr),
        m_RoughMikado(nullptr),
        m_FindKaguya(nullptr),
        m_MountFuji(nullptr)
    {
    }

    void GradDemo::Start()
    {
#ifdef _DEBUG
        // 時間のオフセット
        GraphicsMain::GetInstance()->m_SecondsTimeOffset = 149.0f;// シーンを飛ばすためのオフセット

        // 音楽のミュート
        GraphicsMain::GetInstance()->m_SoundPlayer->Mute(true);

        // デバッグ用
        /*{
            m_DebugTimeLock = true;
            if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
        }*/
#endif
        m_MoonSea = std::make_shared<MoonSea>();
        m_EarthAndMoon = std::make_shared<EarthAndMoon>();
        m_MoonTravel = std::make_shared<MoonTravel>();
        m_CyberpunkSpaceRay = std::make_shared<CyberpunkSpaceRay>();
        m_ChangeOfMind = std::make_shared<ChangeOfMind>();
        m_RoughMikado = std::make_shared<RoughMikado>();
        m_FindKaguya = std::make_shared<FindKaguya>();
        m_MountFuji = std::make_shared<MountFuji>();
    }

    void GradDemo::Load()
    {

    }

    void GradDemo::Update()
    {
        //GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(glm::cos(m_LocalTime) * 3.0f, 0.0f, glm::sin(m_LocalTime) * 3.0f);
        GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(glm::cos(m_LocalTime*0.1f) * 3.0f, 0.0f, glm::sin(m_LocalTime * 0.1f) * 3.0f);

        //GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(1.0f));

        if(m_LocalTime)

        m_MoonSea->Update(m_LocalTime);
        m_EarthAndMoon->Update(m_LocalTime);
        m_MoonTravel->Update(m_LocalTime);
        m_CyberpunkSpaceRay->Update(m_LocalTime);
        m_ChangeOfMind->Update(m_LocalTime);
        m_RoughMikado->Update(m_LocalTime);
        m_FindKaguya->Update(m_LocalTime);
        m_MountFuji->Update(m_LocalTime);
    }

    void GradDemo::Draw(bool IsRaymarching)
    {
        if (m_SceneIndex == 0 || m_SceneIndex == 1) m_MoonSea->Draw(IsRaymarching);
        if (m_SceneIndex == 2) m_EarthAndMoon->Draw(IsRaymarching);
        if (m_SceneIndex == 3) m_MoonTravel->Draw(IsRaymarching);
        if (m_SceneIndex == 4) m_CyberpunkSpaceRay->Draw(IsRaymarching);
        if (m_SceneIndex == 5) m_ChangeOfMind->Draw(IsRaymarching);
        if (m_SceneIndex == 6) m_RoughMikado->Draw(IsRaymarching);
        if (m_SceneIndex == 8) m_FindKaguya->Draw(IsRaymarching);
        if (m_SceneIndex == 9) m_MountFuji->Draw(IsRaymarching);

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
        else if (m_LocalTime >= 134.0f && m_LocalTime < 149.0f)
        {
            m_SceneIndex = 5;
        }
        else if (m_LocalTime >= 149.0f && m_LocalTime < 164.0f) //149 - 164
        {
            m_SceneIndex = 6;
        }
        else if (m_LocalTime >= 164.0f && m_LocalTime < 179.0f) //164 - 179
        {
            m_SceneIndex = 7;
        }
        else if (m_LocalTime >= 179.0f && m_LocalTime < 194.0f)// 179 - 194   => 194 + (286-271) = 179 => 3m
        {
            m_SceneIndex = 8;
        }
        else if (m_LocalTime >= 194.0f && m_LocalTime < 228.0f) // 3:48 = 3*60+48 = 180+48 = 228
        {
            m_SceneIndex = 9;
        }

        m_SceneIndex = 6; // Debug

#ifdef _DEBUG

        //Console::Log("m_LocalTime: %f\n", m_LocalTime);
#endif // _DEBUG

        m_MoonSea->UpdateTimeLine(m_LocalTime);
        m_EarthAndMoon->UpdateTimeLine(m_LocalTime);
        m_MoonTravel->UpdateTimeLine(m_LocalTime);
        m_CyberpunkSpaceRay->UpdateTimeLine(m_LocalTime);
        m_ChangeOfMind->UpdateTimeLine(m_LocalTime);
        m_RoughMikado->UpdateTimeLine(m_LocalTime);
        m_FindKaguya->UpdateTimeLine(m_LocalTime);
        m_MountFuji->UpdateTimeLine(m_LocalTime);
    }
}