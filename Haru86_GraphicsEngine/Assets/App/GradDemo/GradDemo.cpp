#include "GradDemo.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Text/TextObject.h"
#include "Script/MoonSea/MoonSea.h"

namespace app
{
    GradDemo::GradDemo() :
        m_SceneIndex(0),
        m_TestRenderer(nullptr),
        m_MoonSea(nullptr)
    {
    }

    void GradDemo::Start()
    {
        m_TestRenderer = std::make_shared<MeshRendererComponent>(
            std::make_shared<TransformComponent>(),
            PrimitiveType::SPHERE,
            RenderingSurfaceType::RASTERIZER,
            shaderlib::ShaderLib::Standard_vert,
            shaderlib::ShaderLib::Standard_frag
        );

        m_MoonSea = std::make_shared<app::MoonSea>();
    }

    void GradDemo::Load()
    {

    }

    void GradDemo::Update()
    {
        float t = GraphicsMain::GetInstance()->m_SecondsTime,r = 2.5f;
        GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(r * glm::cos(t), 0.0f,r * glm::sin(t));

        m_MoonSea->Update();
    }

    void GradDemo::Draw(bool IsRaymarching)
    {
        //m_TestRenderer->Draw();
        //text::TextObject::Draw("Haru86_");
        m_MoonSea->Draw(IsRaymarching);
    }

    void GradDemo::UpdateTimeline()
    {
        m_MoonSea->UpdateTimeLine(0.0f);
    }
}