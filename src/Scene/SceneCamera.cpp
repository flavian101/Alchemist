#include "SceneCamera.h"
#include <iostream>
#include "Graphics/Camera/Camera.h"
#include "Graphics/Camera/PerspectiveCamera.h"
#include "Graphics/Camera/FreeLook.h"
#include "Graphics/Camera/FirstPerson.h"
#include "Graphics/Camera/ThirdPerson.h"
#include <Graphics/Camera/OrthographicCamera.h>
#include <Graphics/Graphics.h>
#include "Input/Input.h"
#include "Scene/Player.h"

std::map<std::string, SceneCamera*> SceneCamera::m_cameras;
std::vector<std::string> SceneCamera::m_cameraNames;

SceneCamera::SceneCamera(const std::string& name, Graphics& g)
    : m_name(name),
    m_graphics(g),
    m_perspectiveCamera(nullptr),
    m_orthographicCamera(nullptr),
    isPerspective(true),
    m_selectedCamera(this)
{
    if (isPerspective) {
        m_perspectiveCamera = std::make_unique<FreeLook>();
        //m_perspectiveCamera->SetCamera(45.0f, m_graphics.GetAspectRatio(), 1.0f, 1000.0f);
    }
    else {
        m_orthographicCamera = std::make_unique<OrthographicCamera>();
        m_orthographicCamera->SetCamera(m_graphics.GetWidth(), m_graphics.GetHeight(), 1.0f, 5.0f);
    }

    m_cameras[name] = this;
    m_cameraNames.push_back(name);
    input = std::make_unique<Input>(g.getWin());
}

SceneCamera::~SceneCamera() {

}

void SceneCamera::SetPerspectiveCamera(std::unique_ptr < PerspectiveCamera> newPerspectiveCamera) {
    m_perspectiveCamera.reset();  // Clean up the old camera
    m_perspectiveCamera = std::move(newPerspectiveCamera);
    isPerspective = true;
}

void SceneCamera::SetOrthographicCamera(std::unique_ptr<OrthographicCamera> newOrthographicCamera) {
    m_orthographicCamera.reset();  // Clean up the old camera
    m_orthographicCamera = std::move(newOrthographicCamera);
    isPerspective = false;
}

void SceneCamera::Update(float delta,Player& player) 
{
    if (isPerspective)
    {
        switch (this->GetPerspective()->GetCameraMode())
        {
            case CameraMode::freeLook:
                input->DetectInputFree(delta,
                    dynamic_cast<FreeLook*>(this->GetSelectedCamera()->GetPerspective()));
                break;
            case CameraMode::thirdPerson:
                input->DetectInputThird(delta,
                    dynamic_cast<ThirdPerson*>(this->GetSelectedCamera()->GetPerspective()), player);
                break;
            case CameraMode::firstPerson:
                input->DetectInputFirst(delta,
                    dynamic_cast<FirstPerson*>(this->GetSelectedCamera()->GetPerspective()));
                break;
        }
     
    }
  // m_selectedCamera = GetSelectedCamera();
    if (m_selectedCamera) {
        m_selectedCamera->getActiveCamera()->Update(delta);
    }
}

void SceneCamera::Render() {
   // m_selectedCamera = GetSelectedCamera();
    if (m_selectedCamera) {
        m_graphics.SetViewMatrix(m_selectedCamera->getActiveCamera()->GetView());
        m_graphics.SetProjectionMatrix(m_selectedCamera->getActiveCamera()->GetProjectionMatrix());
    }
}

Camera* SceneCamera::getActiveCamera() const {
    if (isPerspective)
    {
        return m_perspectiveCamera.get();
    }
    else
    {
        return m_orthographicCamera.get();
    }
}

SceneCamera* SceneCamera::GetSelectedCamera()
{
    return m_selectedCamera;
}

bool SceneCamera::isPerspectiveCamera() const {
    return isPerspective;
}

PerspectiveCamera* SceneCamera::GetPerspective() {
    return m_perspectiveCamera.get();
}

OrthographicCamera* SceneCamera::GetOrthographic() {
    return m_orthographicCamera.get();
}

void SceneCamera::CreateNewCamera(const std::string& name, bool perspective) {
    if (m_cameras.find(name) != m_cameras.end()) {
        std::cout << "A camera with the name '" << name << "' already exists." << std::endl;
        return;
    }

    SceneCamera* newCamera = new SceneCamera(name, m_graphics);
   // input = std::make_unique<Input>(m_graphics.getWin());
    m_cameras[name] = newCamera;
    m_cameraNames.push_back(name);
}

const std::map<std::string, SceneCamera*>& SceneCamera::GetCameras() const {
    return m_cameras;
}

void SceneCamera::ControlWindow() 
{
    ImGui::Text("Camera Settings");
    if (ImGui::Button("Create New Camera")) {
        showCreateWindow = true;
    }
    if (showCreateWindow)
    {
        static char cameraName[128] = "";
        ImGui::InputText("Name", cameraName, IM_ARRAYSIZE(cameraName));
        if (ImGui::Button("Add Camera")) {
            CreateNewCamera(cameraName, true);
            cameraName[0] = '\0'; // Clear the input text field after adding the camera
            showCreateWindow = false; // Hide the create window after adding the camera
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            showCreateWindow = false; // Hide the create window when cancel button is clicked
        }
    }

    if (m_selectedCamera) {
        const char* items[] = { "Perspective", "Orthographic" };
        static int currentItem = m_selectedCamera->isPerspectiveCamera() ? 0 : 1;

        if (ImGui::BeginCombo("Type", items[currentItem])) {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                bool isSelected = (currentItem == n);
                if (ImGui::Selectable(items[n], isSelected)) {
                    currentItem = n;
                    if (m_selectedCamera) {
                        if (n == 0)
                            m_selectedCamera->SetPerspectiveCamera(std::make_unique<FreeLook>());
                        else
                            m_selectedCamera->SetOrthographicCamera( std::make_unique<OrthographicCamera>());
                    }
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        static const char* cameraModeItems[] = { "Free Look", "Third Person", "First Person" };
        static int selectedMode = static_cast<int>(GetPerspective()->GetCameraMode());

        if (ImGui::Combo("Camera Mode", &selectedMode, cameraModeItems, IM_ARRAYSIZE(cameraModeItems)))
        {
            switch (selectedMode) {
            case 0:
                SetPerspectiveCamera(std::make_unique<FreeLook>());
                break;
            case 1:
                SetPerspectiveCamera(std::make_unique<ThirdPerson>());
                break;
            case 2:
                SetPerspectiveCamera(std::make_unique<FirstPerson>());
                break;
            default:
                // Handle unexpected selection (optional)
                break;
            }
        }
        if (m_selectedCamera->isPerspectiveCamera()) 
        {
            switch (this->GetPerspective()->GetCameraMode())
            {
            case CameraMode::freeLook:
               dynamic_cast<FreeLook*>(m_selectedCamera->GetPerspective())->ControlWindow();
                break;
            case CameraMode::thirdPerson:
                dynamic_cast<ThirdPerson*>(m_selectedCamera->GetPerspective())->ControlWindow();
                break;
            case CameraMode::firstPerson:
                dynamic_cast<FirstPerson*>(m_selectedCamera->GetPerspective())->ControlWindow();
                break;
            }
        }
        else {
            m_selectedCamera->GetOrthographic()->ControlWindow();
        }
    }

 
}
