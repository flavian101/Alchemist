#include "SceneCamera.h"

std::map<std::string, SceneCamera*> SceneCamera::m_cameras;
std::vector<std::string> SceneCamera::m_cameraNames;

SceneCamera::SceneCamera(const std::string& name, Graphics& g, bool perspective)
    : m_name(name),
    m_graphics(g),
    m_perspectiveCamera(nullptr),
    m_orthographicCamera(nullptr),
    isPerspective(perspective),
    m_selectedCamera(this)
{
    if (isPerspective) {
        m_perspectiveCamera = new PerspectiveCamera();
        m_perspectiveCamera->SetCamera(45.0f, m_graphics.getAspectRatio(), 1.0f, 1000.0f);
    }
    else {
        m_orthographicCamera = new OrthographicCamera();
        m_orthographicCamera->SetCamera(m_graphics.getWidth(), m_graphics.getHeight(), 1.0f, 5.0f);
    }

    m_cameras[name] = this;
    m_cameraNames.push_back(name);
}

SceneCamera::~SceneCamera() {
    delete m_perspectiveCamera;
    delete m_orthographicCamera;
}

void SceneCamera::SetPerspectiveCamera(PerspectiveCamera* newPerspectiveCamera) {
    delete m_perspectiveCamera;  // Clean up the old camera
    m_perspectiveCamera = newPerspectiveCamera;
    isPerspective = true;
}

void SceneCamera::SetOrthographicCamera(OrthographicCamera* newOrthographicCamera) {
    delete m_orthographicCamera;  // Clean up the old camera
    m_orthographicCamera = newOrthographicCamera;
    isPerspective = false;
}

void SceneCamera::Update(float delta) {
    m_selectedCamera = this->GetSelectedCamera();
    if (m_selectedCamera) {
        m_selectedCamera->getActiveCamera()->Update(delta);
    }
}

void SceneCamera::Render() {
    m_selectedCamera = this->GetSelectedCamera();
    if (m_selectedCamera) {
        m_graphics.SetViewMatrix(m_selectedCamera->getActiveCamera()->GetView());
        m_graphics.SetProjectionMatrix(m_selectedCamera->getActiveCamera()->GetProjectionMatrix());
    }
}

Camera* SceneCamera::getActiveCamera() const {
    if (isPerspective)
    {
        return m_perspectiveCamera;
    }
    else
    {
        return m_orthographicCamera;
    }
}

SceneCamera* SceneCamera::GetSelectedCamera() const {
    return m_selectedCamera;
}

bool SceneCamera::isPerspectiveCamera() const {
    return isPerspective;
}

PerspectiveCamera* SceneCamera::GetPerspective() {
    return m_perspectiveCamera;
}

OrthographicCamera* SceneCamera::GetOrthographic() {
    return m_orthographicCamera;
}

void SceneCamera::CreateNewCamera(const std::string& name, bool perspective) {
    if (m_cameras.find(name) != m_cameras.end()) {
        std::cout << "A camera with the name '" << name << "' already exists." << std::endl;
        return;
    }

    SceneCamera* newCamera = new SceneCamera(name, m_graphics, perspective);
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

        if (m_selectedCamera->isPerspectiveCamera()) {
            m_selectedCamera->GetPerspective()->ControlWindow();
        }
        else {
            m_selectedCamera->GetOrthographic()->ControlWindow();
        }
    }

 
}
