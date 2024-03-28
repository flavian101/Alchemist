#pragma once
#include "window\ImguiManager.h"
#include <DirectXMath.h>

namespace Math {
	using namespace DirectX;

    inline void ImGuiDragXMVector3(const char* label, XMVECTOR& vector)
    {
        XMFLOAT3 float3;
        XMStoreFloat3(&float3, vector);

        // Pass the XMFLOAT3 by reference
        if (ImGui::DragFloat3(label, &float3.x, 1.0f))
        {
            // Update the XMVECTOR from the modified XMFLOAT3
            vector = XMLoadFloat3(&float3);
        }
    }
    inline XMFLOAT3 XMVectorToFloat3(const XMVECTOR& vector)
    {
        XMFLOAT3 result;
        XMStoreFloat3(&result, vector);
        return result;
    }
}