#include "SceneSerializer.h"
#include <fstream>
#include "window/Window.h"
#include "Scene/Scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Camera/PerspectiveCamera.h"
#include "Graphics/Camera/FreeLook.h"
#include "Graphics/Camera/FirstPerson.h"
#include "Graphics/Camera/ThirdPerson.h"
#include "Graphics/Camera/OrthographicCamera.h"
#include "models/vertex.h"
#include "models/Model.h"
#include "Scene/SceneCamera.h"
#include "Scene/Shaders/ShaderManager.h"
#include "Graphics/Renderer.h"
#include "ModelExporter.h"
#include "models/ModelLoader.h"
#include "models/Node.h"
#include <filesystem> 

namespace fs = std::filesystem;

SceneSerializer::SceneSerializer(Graphics& g)
	:
	m_graphics(g)
{}
nlohmann::json SceneSerializer::Serialize(Scene* scene, const std::string& projectDir,const std::string& projectName)
{
	nlohmann::json j;
	j["projectName"] = projectName;
	j["Scene Name"] = scene->GetName();
	//j["device"] = SerializeDevice(*res);
	//j["window"] = SerializeWindow(*Window::get());
	j["SceneCamera"] = SerializeSceneCamera(scene->sceneCamera);
	//j["SceneShader"] = SerializeSceneShader(scene->shaders);

	std::string modelsDir = projectDir + "/Models/";
	std::filesystem::create_directories(modelsDir);

	j["models"] = nlohmann::json::array();
	for (auto& object : scene->objects) 
	{
		if (auto model = dynamic_cast<Model*>(object))
		{
			ModelExporter exporter(*model);
			std::string modelPath = modelsDir + model->GetName() + ".glb";
			exporter.Export(modelPath);
			nlohmann::json modelJson;
			modelJson["name"] = model->GetName();
			modelJson["path"] = modelPath;
			j["models"].push_back(modelJson);
		}
	}
	
	return j;
	
}

void SceneSerializer::Deserialize(Scene* scene,nlohmann::json j)
{
	scene->SetName(j["Scene Name"]);
	//Renderer* res = Window::GetInstance();
	//DeserializeDevice(*res, j["device"]);
	//DeserializeGraphics(Window::GetInstance(), j["graphics"]);
	//DeserializeWindow(*Window::GetInstance(), j["window"]);
	DeserializeSceneCamera(scene->sceneCamera, j["SceneCamera"]);
	////DeserializeSceneShader(scene->defaultShader, j["defaultShader"]);
	//DeserializeSceneShader(scene->texturedShader, j["texturedShader"]);

			//clean 
	scene->objects.clear();

	//load 
	for (const auto& modelJson : j["models"]) {
		std::string modelPath = modelJson["path"];
		std::string modelName = modelJson["name"];
		ModelLoader* model = new ModelLoader(modelPath,modelName, m_graphics);
		scene->AddGameObject(model);
	}

                      
}


nlohmann::json SceneSerializer::SerializeDevice(const Renderer& device)
{
	nlohmann::json j;	
	j["isVsyncEnabled"] = device.isVsyncEnabled;
	j["enableMsaa"] = device.enabledMsaa;
	return j;
}

nlohmann::json SceneSerializer::SerializeGraphics(Graphics& graphics)
{
	nlohmann::json j;
	j["imguiEnabled"] = graphics.imguiEnabled;
	j["projectionMatrix"] = {
		graphics.GetProjectionMatrix().r[0].m128_f32[0], graphics.GetProjectionMatrix().r[0].m128_f32[1], graphics.GetProjectionMatrix().r[0].m128_f32[2], graphics.GetProjectionMatrix().r[0].m128_f32[3],
		graphics.GetProjectionMatrix().r[1].m128_f32[0], graphics.GetProjectionMatrix().r[1].m128_f32[1], graphics.GetProjectionMatrix().r[1].m128_f32[2], graphics.GetProjectionMatrix().r[1].m128_f32[3],
		graphics.GetProjectionMatrix().r[2].m128_f32[0], graphics.GetProjectionMatrix().r[2].m128_f32[1], graphics.GetProjectionMatrix().r[2].m128_f32[2], graphics.GetProjectionMatrix().r[2].m128_f32[3],
		graphics.GetProjectionMatrix().r[3].m128_f32[0], graphics.GetProjectionMatrix().r[3].m128_f32[1], graphics.GetProjectionMatrix().r[3].m128_f32[2], graphics.GetProjectionMatrix().r[3].m128_f32[3]
	};
	j["viewMatrix"] = {
		graphics.GetViewMatrix().r[0].m128_f32[0], graphics.GetViewMatrix().r[0].m128_f32[1], graphics.GetViewMatrix().r[0].m128_f32[2], graphics.GetViewMatrix().r[0].m128_f32[3],
		graphics.GetViewMatrix().r[1].m128_f32[0], graphics.GetViewMatrix().r[1].m128_f32[1], graphics.GetViewMatrix().r[1].m128_f32[2], graphics.GetViewMatrix().r[1].m128_f32[3],
		graphics.GetViewMatrix().r[2].m128_f32[0], graphics.GetViewMatrix().r[2].m128_f32[1], graphics.GetViewMatrix().r[2].m128_f32[2], graphics.GetViewMatrix().r[2].m128_f32[3],
		graphics.GetViewMatrix().r[3].m128_f32[0], graphics.GetViewMatrix().r[3].m128_f32[1], graphics.GetViewMatrix().r[3].m128_f32[2], graphics.GetViewMatrix().r[3].m128_f32[3]
	};

	return j;
}

nlohmann::json SceneSerializer::SerializeWindow(Window& win)
{
	nlohmann::json j;
	return j;
}

nlohmann::json SceneSerializer::SerializePerspectiveCamera(PerspectiveCamera* camera)
{
	nlohmann::json j;
	j["FOV"] = camera->m_FOV;;
	j["aspectRatio"] = m_graphics.GetAspectRatio();
	j["nearPlane"] = camera->m_nearPlane;
	j["farPlane"] = camera->m_farPlane;

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, camera->GetPos());
	j["position"] = { position.x, position.y, position.z };

	DirectX::XMFLOAT3 target;
	DirectX::XMStoreFloat3(&target, camera->GetTarget());
	j["target"] = { target.x, target.y, target.z };

	DirectX::XMFLOAT3 up;
	DirectX::XMStoreFloat3(&up, camera->GetUp());
	j["up"] = { up.x, up.y, up.z };

	j["camYaw"] = camera->camYaw;
	j["camPitch"] = camera->camPitch;
	j["camRoll"] = camera->camRoll;
	j["CameraMode"] = camera->GetCameraMode();

	switch (camera->GetCameraMode()) {
	case CameraMode::freeLook:
		j["free look"] = SerializeFreelook(dynamic_cast<FreeLook*>(camera));
		break;
	case CameraMode::thirdPerson:
		j["thirdPerson"] = SerializeThirdPerson(dynamic_cast<ThirdPerson*>(camera));
		break;
	case CameraMode::firstPerson:
		j["firstPerson"] = SerializeFirstPerson(dynamic_cast<FirstPerson*>(camera));
		break;
	}

	return j;
	
}

nlohmann::json SceneSerializer::SerializeFreelook(FreeLook* look)
{
	nlohmann::json j;
	j["moveLeftRight"] = look->moveLeftRight;
	j["moveBackForward"] = look->moveBackForward;
	j["moveUpDown"] = look->moveUpDown;
	j["cameraSpeed"] = look->cameraSpeed;

	return j;
}

nlohmann::json SceneSerializer::SerializeThirdPerson(ThirdPerson* camera)
{
	nlohmann::json j;
	j["FollowPlayer"] = camera->FollowPlayer;
	j["distance"] = camera->distance;
	j["height"] = camera->height;
	j["cameraLag"] = camera->cameraLag;
	j["moveSpeed"] = camera->moveSpeed;
	j["rotationSpeed"] = camera->rotationSpeed;

	return j;
}

nlohmann::json SceneSerializer::SerializeFirstPerson(FirstPerson* camera)
{
	nlohmann::json j;
	j["movementSpeed"] = camera->movementSpeed;
	j["mouseSensitivity"] = camera->mouseSensitivity;
	return j;
}

nlohmann::json SceneSerializer::SerializeOrthographicCamera(OrthographicCamera* camera)
{
	nlohmann::json j;
	return j;
}

nlohmann::json SceneSerializer::SerializeSceneCamera(SceneCamera* camera)
{
	nlohmann::json j;
	nlohmann::json camerasArray = nlohmann::json::array();
	for (const auto& [name, cam] : camera->m_cameras)
	{
		nlohmann::json cameraObj;
		cameraObj["Camera Name"] = name;
		cameraObj["IsPerspective"] = cam->isPerspective;
		if (cam->isPerspective)
		{
			cameraObj["Perspective Camera"] = SerializePerspectiveCamera(cam->GetPerspective());
		}
		else
		{
			cameraObj["Orthographic Camera"] = SerializeOrthographicCamera(cam->GetOrthographic());
		}
		cameraObj["showCreateWindow"] = cam->showCreateWindow;
		camerasArray.push_back(cameraObj);
	}
	j["Cameras"] = camerasArray;
	return j;
}

nlohmann::json SceneSerializer::SerializeSceneShader(std::vector<std::shared_ptr<ShaderManager>> shader)
{
	nlohmann::json j;
	return nlohmann::json();
}




void SceneSerializer::DeserializeDevice(Renderer& device, const nlohmann::json& j)
{
	device.enabledMsaa = j["enableMsaa"];
	device.isVsyncEnabled = j["isVsyncEnabled"];
}
void SceneSerializer::DeserializeGraphics(Graphics& graphics, const nlohmann::json& j)
{
	
	graphics.imguiEnabled = j["imguiEnabled"];
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMATRIX(
		j["projectionMatrix"][0], j["projectionMatrix"][1], j["projectionMatrix"][2], j["projectionMatrix"][3],
		j["projectionMatrix"][4], j["projectionMatrix"][5], j["projectionMatrix"][6], j["projectionMatrix"][7],
		j["projectionMatrix"][8], j["projectionMatrix"][9], j["projectionMatrix"][10], j["projectionMatrix"][11],
		j["projectionMatrix"][12], j["projectionMatrix"][13], j["projectionMatrix"][14], j["projectionMatrix"][15]
	);
	graphics.SetProjectionMatrix(projectionMatrix);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		j["viewMatrix"][0], j["viewMatrix"][1], j["viewMatrix"][2], j["viewMatrix"][3],
		j["viewMatrix"][4], j["viewMatrix"][5], j["viewMatrix"][6], j["viewMatrix"][7],
		j["viewMatrix"][8], j["viewMatrix"][9], j["viewMatrix"][10], j["viewMatrix"][11],
		j["viewMatrix"][12], j["viewMatrix"][13], j["viewMatrix"][14], j["viewMatrix"][15]
	);
	graphics.SetViewMatrix(viewMatrix);
}

void  SceneSerializer::DeserializeWindow(Window& win, const nlohmann::json& j)
{
	
	
}

void SceneSerializer::DeserializePerspectiveCamera(PerspectiveCamera* camera, const nlohmann::json& j)
{
	camera->setCamera(j["FOV"], j["aspectRatio"],j["nearPlane"], j["farPlane"]);
	DirectX::XMFLOAT3 position(j["position"][0], j["position"][1], j["position"][2]);
	camera->SetPosition(DirectX::XMLoadFloat3(&position));

	DirectX::XMFLOAT3 target(j["target"][0], j["target"][1], j["target"][2]);
	camera->SetTarget(DirectX::XMLoadFloat3(&target));

	DirectX::XMFLOAT3 up(j["up"][0], j["up"][1], j["up"][2]);
	camera->SetUP(DirectX::XMLoadFloat3(&up));

	camera->camYaw = j["camYaw"];
	camera->camPitch =j["camPitch"];
	camera->camRoll = j["camRoll"];


	switch (camera->GetCameraMode()) {
	case CameraMode::freeLook:
		DeserializeFreelook(reinterpret_cast<FreeLook*>(camera), j["free look"]);
		break;
	case CameraMode::thirdPerson:
		DeserializeThirdPerson(reinterpret_cast<ThirdPerson*>(camera), j["thirdPerson"]);
		break;
	case CameraMode::firstPerson:
		DeserializeFirstPerson(reinterpret_cast<FirstPerson*>(camera), j["firstPerson"]);
		break;
	}

}

void SceneSerializer::DeserializeFreelook(FreeLook* look, const nlohmann::json& j)
{
	look->moveLeftRight=(j["moveLeftRight"]);
	look->SetMoveBackForward(j["moveBackForward"]);
	look->SetMoveUpDown(j["moveUpDown"]);
	look->cameraSpeed = j["cameraSpeed"];

}

void SceneSerializer::DeserializeThirdPerson(ThirdPerson* camera, const nlohmann::json& j)
{
	camera->FollowPlayer = j["FollowPlayer"];
	camera->distance = j["distance"];
	camera->height = j["height"];
	camera->cameraLag = j["cameraLag"];
	camera->moveSpeed = j["moveSpeed"];
	camera->rotationSpeed = j["rotationSpeed"];
}

void SceneSerializer::DeserializeFirstPerson(FirstPerson* camera, const nlohmann::json& j)
{
	camera->movementSpeed = j["movementSpeed"];
	camera->mouseSensitivity = j["mouseSensitivity"];
}

void SceneSerializer::DeserializeOrthographicCamera(OrthographicCamera* camera, const nlohmann::json& j)
{
}

void SceneSerializer::DeserializeSceneCamera(SceneCamera* camera, const nlohmann::json& j)
{
	for (const auto& cameraObj : j["Cameras"])
	{
		std::string name = cameraObj["Camera Name"];
		bool isPerspective = cameraObj["IsPerspective"];
		SceneCamera* cam = new SceneCamera(name, m_graphics);
		cam->isPerspective = isPerspective;
	
		if (isPerspective)
		{
			std::unique_ptr<PerspectiveCamera> perspectiveCamera = std::make_unique< PerspectiveCamera>();
			DeserializePerspectiveCamera(perspectiveCamera.get(), cameraObj["Perspective Camera"]);
			cam->SetPerspectiveCamera(std::move(perspectiveCamera));
		}
		else
		{
			OrthographicCamera* orthographicCamera = new OrthographicCamera();
			DeserializeOrthographicCamera(orthographicCamera, cameraObj["Orthographic Camera"]);
			//cam->SetOrthographicCamera(orthographicCamera);
		}
	
		cam->showCreateWindow = cameraObj["showCreateWindow"];
		camera->m_cameras[name] = cam;
	}
}

void SceneSerializer::DeserializeSceneShader(std::shared_ptr<ShaderManager> shader, const nlohmann::json& j)
{
}

