#include "CameraController.h"



void CameraController::CameraShake(PerspectiveCamera* cam, float intensity, float duration, float deltaTime)
{
    DirectX::XMVECTOR camPosition = cam->GetPos();

    float shakeTime = 0.0f;

    while (shakeTime < duration)
    {
        // Generate random displacement vector within intensity range
        DirectX::XMVECTOR shakeOffset = DirectX::XMVectorSet(
            RandomFloat(-intensity, intensity),
            RandomFloat(-intensity, intensity),
            RandomFloat(-intensity, intensity),
            0.0f
        );

        // Apply the shake offset to the camera's position
        camPosition += shakeOffset;

        // Update camera position
        cam->SetPosition(camPosition);

        // Increment shake time
        shakeTime += deltaTime;
    }
}

float CameraController::RandomFloat(float min, float max)
{
    // Create a random device to seed the random number generator
    std::random_device rd;

    // Create a Mersenne Twister pseudo-random number generator engine
    std::mt19937 gen(rd());

    // Create a uniform real distribution to generate floats in the specified range
    std::uniform_real_distribution<float> dis(min, max);

    // Generate and return a random float
    return dis(gen);
}