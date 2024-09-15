#pragma once
#include "Graphics\ConstantBuffer.h"

class Graphics;
/// <summary>
/// ///set the Vertex constant buffer
/// </summary>



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
    void setTransform(const FXMMATRIX& matrix);

    // Update method (e.g., for animation)
    void Update(float time);
    void UpdateFromTransform(float Time);
        void BindConstantBuffer();

        //imgui window
        void controlWindow();
        void PrintMatrix(const char* name, const XMMATRIX& matrix);

private:


private:
    XMMATRIX m_transform;
    XMVECTOR m_position;
    XMVECTOR m_rotation;
    XMVECTOR m_scale;
    ConstantBuffer< cb_vsConstantBuffer> CB_Buffer;
    Graphics& m_graphics;
};

