#include "CameraController.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

void CameraController::CameraShake(PerspectiveCamera* cam, float intensity, float duration, float deltaTime)
{
    static float shakeTime = 0.0f; // Declare as static to persist between function calls

    // Generate random displacement vector within intensity range
    DirectX::XMVECTOR shakeOffset = DirectX::XMVectorSet(
        RandomFloat(-intensity, intensity),
        RandomFloat(-intensity, intensity),
        RandomFloat(-intensity, intensity),
        0.0f
    );

    // Apply the shake offset to the camera's position
    DirectX::XMVECTOR camPosition = cam->GetPos() + shakeOffset;

    // Update camera position
    cam->SetPosition(camPosition);

    // Increment shake time
    shakeTime += deltaTime;

    // Reset shake time and position if duration is exceeded
    if (shakeTime >= duration)
    {
        shakeTime = 0.0f;
        cam->Reset(); // Reset camera position to its original state or any desired position
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