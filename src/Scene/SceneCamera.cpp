// SceneCamera.cpp

#include "SceneCamera.h"

std::map<std::string, SceneCamera*> SceneCamera::m_cameras;

SceneCamera::SceneCamera(const std::string& name, PerspectiveCamera* perspectiveCamera)
    : m_name(name),
    m_perspectiveCamera(perspectiveCamera),
    m_orthographicCamera(nullptr),
    isPerspective(true)
{
    m_cameras[name] = this;
    m_cameraNames.push_back(name);
}

SceneCamera::SceneCamera(const std::string& name, OrthographicCamera* orthographicCamera)
    : m_name(name),
    m_perspectiveCamera(nullptr),
    m_orthographicCamera(orthographicCamera),
    isPerspective(false)
{
    m_cameras[name] = this;
    m_cameraNames.push_back(name);
}

SceneCamera::~SceneCamera()
{
    // Clean up your cameras here
    //delete m_perspectiveCamera;
   // delete m_orthographicCamera;
   // delete m_selectedCamera;

}

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

void SceneCamera::Update(float delta)
{
    
}

void SceneCamera::Render()
{
}

Camera* SceneCamera::getActiveCamera() const
{
    if (isPerspective)
        return m_perspectiveCamera;
    else
        return m_orthographicCamera;
}

SceneCamera* SceneCamera::GetSelectedCamera() const
{
    return m_selectedCamera;
}

bool SceneCamera::isPerspectiveCamera()
{
    return isPerspective;
}

PerspectiveCamera* SceneCamera::GetPerspective()
{
    return m_perspectiveCamera;
}

OrthographicCamera* SceneCamera::GetOrthographic()
{
    return m_orthographicCamera;
}
// SceneCamera.cpp
void SceneCamera::CreateNewCamera(const std::string& name, bool perspective)
{
    // Check if the camera already exists
    if (m_cameras.find(name) != m_cameras.end())
    {
        std::cout << "A camera with the name '" << name << "' already exists." << std::endl;
        return;
    }

    // Create the new camera based on the perspective flag
    SceneCamera* newCamera;
    if (perspective)
    {
        newCamera = new SceneCamera(name, new PerspectiveCamera());
    }
    else
    {
        newCamera = new SceneCamera(name, new OrthographicCamera());
    }

    // Add the camera to the map
    m_cameras[name] = newCamera;

    // Add the camera name to the vector
    m_cameraNames.push_back(name);
}

const std::map<std::string, SceneCamera*>& SceneCamera::GetCameras()
{
    return m_cameras;
}

// SceneCamera.cpp
void SceneCamera::ControlWindow()
{
    ImGui::Begin("Camera");

    // Camera Selection
    const std::map<std::string, SceneCamera*>& cameras = GetCameras();

    ImGui::Text("Select Camera:");
    ImGui::BeginChild("CameraSelection", ImVec2(0, 100), true);
    for (const auto& [name, camera] : cameras)
    {
        bool isSelected = (m_selectedCamera == camera);
        if (ImGui::Selectable(name.c_str(), isSelected))
        {
            if (isSelected)
                m_selectedCamera = nullptr; // Deselect the camera
            else
                m_selectedCamera = camera; // Select the camera
        }
    }
    ImGui::EndChild();


    ImGui::Separator();

    // Button to create a new camera
    if (ImGui::Button("Create New Camera"))
    {
        ImGui::OpenPopup("New Camera Popup");
    }

    // New camera pop-up
    if (ImGui::BeginPopupModal("New Camera Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char cameraName[128] = "";
        ImGui::InputText("Name", cameraName, IM_ARRAYSIZE(cameraName));

        static int cameraType = 0;
        const char* cameraTypes[] = { "Perspective", "Orthographic" };
        ImGui::Combo("Type", &cameraType, cameraTypes, IM_ARRAYSIZE(cameraTypes));

        if (ImGui::Button("Add Camera"))
        {
            if (cameraType == 0)
                CreateNewCamera(cameraName, true); // Perspective
            else
                CreateNewCamera(cameraName, false); // Orthographic

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    // Camera Type Selection
    const char* items[] = { "Perspective", "Orthographic" };
    static int currentItem = m_selectedCamera && m_selectedCamera->isPerspectiveCamera() ? 0 : 1;
   
    if (ImGui::BeginCombo("Type", items[currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool isSelected = (currentItem == n);
            if (ImGui::Selectable(items[n], isSelected))
            {
                currentItem = n;
                if (m_selectedCamera)
                {
                    if (n == 0)
                        m_selectedCamera->SetPerspectiveCamera(new PerspectiveCamera());
                    else
                        m_selectedCamera->SetOrthographicCamera(new OrthographicCamera());
                }
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Display camera settings based on the selected camera type
    if (m_selectedCamera && m_selectedCamera->isPerspectiveCamera())
    {
        // Assuming GetPerspective() returns a PerspectiveCamera pointer
        m_selectedCamera->GetPerspective()->ControlWindow();
    }
    else if (m_selectedCamera && !m_selectedCamera->isPerspectiveCamera())
    {
        // Assuming GetOrthographic() returns an OrthographicCamera pointer
        m_selectedCamera->GetOrthographic()->ControlWindow();
    }

    ImGui::End();
}