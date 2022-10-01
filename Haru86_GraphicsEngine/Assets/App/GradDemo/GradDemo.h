#pragma once
#include <memory>

class MeshRendererComponent;

namespace text {class TextObject;}

namespace app
{
    class GradDemo
    {
        //
        std::shared_ptr<MeshRendererComponent> m_TestRenderer;

        //
        unsigned int m_SceneIndex;
    public:
        GradDemo();
        ~GradDemo() = default;

        void Start();
        void Load();
        void Update();
        void Draw(bool IsRaymarching);
        void UpdateTimeline();

        unsigned int GetSceneIndex()const { return m_SceneIndex; }
    };
}

