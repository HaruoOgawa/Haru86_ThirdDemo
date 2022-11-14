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
    public:
        BoidsGPGPU();
        virtual ~BoidsGPGPU();

        void Start();
        void Update();
        void Draw();
        float rand(glm::vec2 st);
	};
}