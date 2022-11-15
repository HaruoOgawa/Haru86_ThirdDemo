#pragma once
#include <memory>
#include <glm/glm.hpp>

class Material;
class ComputeBuffer;

namespace app
{
    struct BoidsForce {
        float velocity[4];
        BoidsForce(float v[4]) {
            velocity[0] = v[0];
            velocity[1] = v[1];
            velocity[2] = v[2];
            velocity[3] = v[3];
        }
    };

    struct BoidsData {
        float position[4];
        float velocity[4];
        float viewDir[4];
        BoidsData(float p[4], float v[4]) {
            position[0] = p[0];
            position[1] = p[1];
            position[2] = p[2];
            position[3] = p[3];

            velocity[0] = v[0];
            velocity[1] = v[1];
            velocity[2] = v[2];
            velocity[3] = v[3];

            viewDir[0] = p[0];
            viewDir[1] = p[1];
            viewDir[2] = p[2];
            viewDir[3] = p[3];
        }
    };

	class BoidsGPGPU
	{
        std::shared_ptr<ComputeBuffer> m_BoidsForce_CB;
        std::shared_ptr<Material> m_BoidsForce_CS;
        std::shared_ptr<ComputeBuffer> m_BoidsDataCB;
        std::shared_ptr<Material> m_BoidsDataCS;

        int m_BoidsNum;
        int m_NumOfThreads;

        unsigned int m_BufferBoidsForceBinding;
        unsigned int m_BufferBoidsDataBinding;
    private:
        float rand(glm::vec2 st);
        void InitBuffer();
        void Dispatch();
    public:
        BoidsGPGPU(int BoidsNum, int NumOfThreads, unsigned int BufferBoidsForceBinding, unsigned int BufferBoidsDataBinding);
        virtual ~BoidsGPGPU();

        void Start();
        void Update(float time);
        void Draw();

        unsigned int GetBufferBoidsForceBinding() const { return m_BufferBoidsForceBinding; }
        unsigned int GetBufferBoidsDataBinding() const { return m_BufferBoidsDataBinding; }

        const std::shared_ptr<ComputeBuffer>& GetBoidsForce_CB()const { return m_BoidsForce_CB; }
        const std::shared_ptr<ComputeBuffer>& GetBoidsDataCB()const { return m_BoidsDataCB; }
	};
}