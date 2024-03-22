#include "SceneCamera.h"

SceneCamera::SceneCamera(OrthographicCamera* orthographicCamera)
    : m_orthographicCamera(orthographicCamera),
      m_perspectiveCamera(nullptr),
      isPerspective(false)
{}
SceneCamera::SceneCamera(PerspectiveCamera* perspectiveCamera)
    : m_perspectiveCamera(perspectiveCamera),
      m_orthographicCamera(nullptr),
      isPerspective(true)
{}
void SceneCamera::SetPerspectiveCamera(PerspectiveCamera* newPerspectiveCamera)
{
    m_perspectiveCamera = newPerspectiveCamera;
    isPerspective = true;
}

void SceneCamera::SetOrthographicCamera(OrthographicCamera* newOrthographicCamera)
{
    m_orthographicCamera = newOrthographicCamera;
    isPerspective = false;
}

Camera* SceneCamera::getActiveCamera() const
{
    if (isPerspective)
        return m_perspectiveCamera;
    else
        return m_orthographicCamera;
}


bool SceneCamera::isPerspectiveCamera()
{
    return isPerspective;
}



PerspectiveCamera* SceneCamera::GetPerspective()
{
    return m_perspectiveCamera;
}

OrthographicCamera* SceneCamera::GetOthorgraphic()
{
 
    return m_orthographicCamera;
}


void SceneCamera::ControlWindow()
{
    ImGui::Begin("Camera type");

    const char* items[] = { "Perspective", "Orthographic" };
    static int currentItem = isPerspective ? 0 : 1; // Initialize based on current camera type

    if (ImGui::BeginCombo("Type", items[currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (currentItem == n); // Check against loop index directly
            if (ImGui::Selectable(items[n], is_selected))
            {
                currentItem = n; // Update the selected item index directly
                isPerspective = (n == 0); // Update camera type
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus(); // Set initial focus when opening the combo
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}