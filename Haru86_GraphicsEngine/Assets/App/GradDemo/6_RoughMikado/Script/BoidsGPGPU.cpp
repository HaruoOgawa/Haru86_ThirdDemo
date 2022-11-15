#include "BoidsGPGPU.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace app
{
    BoidsGPGPU::BoidsGPGPU(int BoidsNum, int NumOfThreads, unsigned int BufferBoidsForceBinding, unsigned int BufferBoidsDataBinding):
        m_BoidsNum(BoidsNum),
        m_BoidsForce_CB(nullptr),
        m_BoidsForce_CS(nullptr),
        m_BoidsDataCB(nullptr),
        m_BoidsDataCS(nullptr),
        m_BufferBoidsForceBinding(BufferBoidsForceBinding),
        m_BufferBoidsDataBinding(BufferBoidsDataBinding),
        m_NumOfThreads(NumOfThreads)

    {
        //
        m_BoidsForce_CB = std::make_shared<ComputeBuffer>(sizeof(BoidsForce) * m_BoidsNum);
        m_BoidsForce_CS = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, "", "", "", "", "", 
            "#version 430\n"
            "#define NUMTHREADS_X_NUM " + std::to_string(NumOfThreads) + "\n"
            "#define BufferBoidsForceBinding "+ std::to_string(BufferBoidsForceBinding) +"\n"+
            "#define BufferBoidsDataBinding "+ std::to_string(BufferBoidsDataBinding) +"\n"+
            std::string(
                #include "../Shader/BoidsForce.comp"
            )
        );
        m_BoidsDataCB = std::make_shared<ComputeBuffer>(sizeof(BoidsData) * m_BoidsNum);
        m_BoidsDataCS = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, "", "", "", "", "",
            "#version 430\n"
            "#define NUMTHREADS_X_NUM "+ std::to_string(NumOfThreads) +"\n"
            "#define BufferBoidsForceBinding " + std::to_string(BufferBoidsForceBinding) + "\n" +
            "#define BufferBoidsDataBinding " + std::to_string(BufferBoidsDataBinding) + "\n" +
            std::string(
                #include "../Shader/BoidsData.comp"
            )
        );
        
        //
        InitBuffer();

        //
        Start();
    }
    BoidsGPGPU::~BoidsGPGPU()
    {
    }

    void BoidsGPGPU::InitBuffer()
    {
        std::vector<BoidsForce> InitForce;
        std::vector<BoidsData> InitData;

        for (int i = 0; i < m_BoidsNum; i++) {
            float size = 1.0f;
            float pos[4] = {
                (rand(glm::vec2(i,0.951f)) * 2.0f - 1.0f) * size,
                (rand(glm::vec2(i,0.456f)) * 2.0f - 1.0f) * size,
                (rand(glm::vec2(i,0.753f)) * 2.0f - 1.0f) * size,
                1.0f
            };

            glm::vec3 v = glm::normalize(glm::vec3(
                (rand(glm::vec2(0.321, i)) * 2.0 - 1.0),
                (rand(glm::vec2(0.654, i)) * 2.0 - 1.0),
                (rand(glm::vec2(0.987, i)) * 2.0 - 1.0)
            ));
            float vel[4] = {
                v.x,
                v.y,
                v.z,
                0.0f
            };

            //force
            InitForce.push_back(BoidsForce(vel));

            //data
            InitData.push_back(BoidsData(pos, vel));
        }

        //force attach
        m_BoidsForce_CB->SetData<std::vector<BoidsForce>>(InitForce);

        //data attach
        m_BoidsDataCB->SetData<std::vector<BoidsData>>(InitData);

        //SetBufferToCS//////////////
        //force compute
        m_BoidsForce_CS->SetBufferToCS(m_BoidsForce_CB, m_BufferBoidsForceBinding);
        m_BoidsForce_CS->SetBufferToCS(m_BoidsDataCB, m_BufferBoidsDataBinding);
        //data compute
        m_BoidsDataCS->SetBufferToCS(m_BoidsForce_CB, m_BufferBoidsForceBinding);
        m_BoidsDataCS->SetBufferToCS(m_BoidsDataCB, m_BufferBoidsDataBinding);

        //clear data
        InitForce.clear();
        InitData.clear();
    }

    void BoidsGPGPU::Dispatch()
    {
        //Dispatch force///////////////////////////
        {
            m_BoidsForce_CS->SetActive();
            m_BoidsForce_CS->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
            m_BoidsForce_CS->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
            //boid uniform
            m_BoidsForce_CS->SetFloatUniform("_centerPosPower", 1000.0f);
            m_BoidsForce_CS->SetFloatUniform("_maxBoidsDist", 50);

            m_BoidsForce_CS->SetIntUniform("_boids_count", m_BoidsNum);
            m_BoidsForce_CS->SetFloatUniform("_maxBoidsField", 200);
            m_BoidsForce_CS->SetFloatUniform("_NoiseValue", 0.1);
            m_BoidsForce_CS->SetFloatUniform("_joinPower", 100);
            m_BoidsForce_CS->SetFloatUniform("_separatePower", 1000);
            m_BoidsForce_CS->SetFloatUniform("_alignmentPower", 100);
            m_BoidsForce_CS->SetFloatUniform("_joinDisMin", 0.1);  // pre10
            m_BoidsForce_CS->SetFloatUniform("_separateDistMin", 1000); //pre10
            m_BoidsForce_CS->SetFloatUniform("_alignmentDistMin", 0.1f); //pre10
            m_BoidsForce_CS->Dispatch(m_BoidsNum / m_NumOfThreads, 1, 1);
        }

        //Dispatch Data///////////////
        {
            m_BoidsDataCS->SetActive();
            m_BoidsDataCS->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
            m_BoidsDataCS->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
            //boid uniform
            m_BoidsDataCS->Dispatch(m_BoidsNum / m_NumOfThreads, 1, 1);
        }
    }

    void BoidsGPGPU::Start()
    {
        Dispatch();
    }

    void BoidsGPGPU::Update(float time)
    {
        Dispatch();

#ifdef _DEBUG
        if (GraphicsMain::GetInstance()->m_ShowDebugLog)
        {
            float p[4] = { 0.0f,0.0f,0.0f,0.0f }; float v[4] = { 0.0f,0.0f,0.0f,0.0f };
            std::vector<BoidsData> Data(m_BoidsNum, BoidsData(p, v));
            m_BoidsDataCB->GetBufferData<BoidsData>(&Data[0], 0, m_BoidsNum);
            Console::Log("____________________________________________________\n");
            for (int i=0;i<Data.size();i++)
            {
                const auto& val = Data[i];
                Console::Log("%d / position=> x:%f, y:%f, z:%f, w:%f\n", i, val.position[0], val.position[1], val.position[2], val.position[3]);
            }
        }
#endif // _DEBUG

    }

    void BoidsGPGPU::Draw()
    {

    }

    float BoidsGPGPU::rand(glm::vec2 st)
    {
        return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453);
    }
}