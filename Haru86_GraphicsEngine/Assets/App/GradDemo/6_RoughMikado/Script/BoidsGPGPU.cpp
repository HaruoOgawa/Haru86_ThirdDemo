#include "BoidsGPGPU.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"

namespace app
{
    BoidsGPGPU::BoidsGPGPU():
        m_BoidsNum(1024),
        m_BoidsForce_CB(nullptr),
        m_BoidsForce_CS(nullptr),
        m_BoidsDataCB(nullptr),
        m_BoidsDataCS(nullptr)
    {
        //
        m_BoidsForce_CB = std::make_shared<ComputeBuffer>(sizeof(BoidsForce) * m_BoidsNum);
        m_BoidsDataCB = std::make_shared<ComputeBuffer>(sizeof(BoidsData) * m_BoidsNum);
        //m_BoidsForce_CB
        
        //

    }
    BoidsGPGPU::~BoidsGPGPU()
    {

    }

    void BoidsGPGPU::Start()
    {

    }

    void BoidsGPGPU::Update()
    {

    }

    void BoidsGPGPU::Draw()
    {

    }

    float BoidsGPGPU::rand(glm::vec2 st)
    {
        return 1.0f;
    }
}