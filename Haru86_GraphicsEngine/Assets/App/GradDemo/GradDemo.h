#pragma once
#include <memory>

class MeshRendererComponent;

namespace text {class TextObject;}

namespace app
{
    class MoonSea;
    class EarthAndMoon;
    class MoonTravel;

    class GradDemo
    {
        //
        std::shared_ptr<MoonSea> m_MoonSea;
        std::shared_ptr<EarthAndMoon> m_EarthAndMoon;
        std::shared_ptr<MoonTravel> m_MoonTravel;

        //
        unsigned int m_SceneIndex;
        float        m_LocalTime;
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

