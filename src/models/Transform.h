#pragma once
#include <DirectXMath.h>
#include "Graphics\ConstantBuffer.h"
#include "Graphics\Graphics.h"
/// <summary>
/// ///set the Vertex constant buffer
/// </summary>

using namespace DirectX;


class Transform
{
public:
    Transform(Graphics& g,
        const XMFLOAT3& position = XMFLOAT3(0.0f, 0.0f, 0.0f),
        const XMFLOAT4& rotation = XMFLOAT4(0.0f, 0.0f, 0.0f,0.0f),
        const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f));
       

  //  constexpr XMVECTOR Vectorfromfloat3(XMFLOAT3& float3);

    // Setters for position, rotation, and scale
    void SetPosition(const XMFLOAT3& position) { m_position = XMLoadFloat3(&position); }
    void SetRotation(const XMFLOAT4& rotation) { m_rotation = XMLoadFloat4(&rotation); }
    void SetScale(const XMFLOAT3& scale) { m_scale = XMLoadFloat3(&scale); }

    // Getters for position, rotation, and scale
    XMVECTOR GetPosition() const { return m_position; }
    XMVECTOR GetRotation() const { return m_rotation; }
    XMVECTOR GetScale() const { return m_scale; }
    XMMATRIX GetTransform() const;

    // Update method (e.g., for animation)
    void Update(float time);
        // Example: Update position, rotation, etc.
    

    // Render method (might be empty if rendering is handled elsewhere)
    void BindConstantBuffer();
        // Example: Render the entity (might not be implemented here)

private:
    XMMATRIX transform;
    XMVECTOR m_position;
    XMVECTOR m_rotation;
    XMVECTOR m_scale;
    ConstantBuffer< cb_vsConstantBuffer> CB_Buffer;
    Graphics& m_graphics;
};

