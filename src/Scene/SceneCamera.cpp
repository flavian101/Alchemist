#include "SceneCamera.h"

SceneCamera::SceneCamera()    
{
}

PerspectiveCamera SceneCamera::CreatePerspective(float FOV)
{
    m_FOV = FOV;
    return PerspectiveCamera(FOV);
}

OrthographicCamera SceneCamera::CreateOthorgaphic(float width, float height)
{
    m_width = width;
    m_height = height;
    return OrthographicCamera(width, height);
}

void SceneCamera::ControlWindow()
{
    ImGui::Begin("Camera type");

    const char* items[] = { "Perspective", "Otrhographic" };
    static int currentItem = 0; // Use integer index instead of string comparison

    if (ImGui::BeginCombo("Type", items[currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (currentItem == n); // Check against loop index directly
            if (ImGui::Selectable(items[n], is_selected))
                currentItem = n; // Update the selected item index directly
            if (is_selected)
                ImGui::SetItemDefaultFocus(); // Set initial focus when opening the combo
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    // Perform actions based on the selected item after the ImGui window ends
    switch (currentItem)
    {
    case 0:
        CreatePerspective(m_FOV);
        break;
    case 1: 
        CreateOthorgaphic(m_width, m_height);
        break;
    }
}
