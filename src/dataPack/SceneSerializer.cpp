#include "SceneSerializer.h"
#include <fstream>
#include "window/Window.h"
#include "Scene/Scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/DeviceResources.h"
#include "Graphics/Camera/PerspectiveCamera.h"
#include "Graphics/Camera/OrthographicCamera.h"
#include "models/vertex.h"
#include "models/Model.h"
#include "models/Mesh.h"
#include "models/MeshParts.h"
#include "Scene/SceneCamera.h"
#include "Scene/Shaders/ShaderManager.h"


SceneSerializer::SceneSerializer(Scene& scene,Graphics& g)
	:
	m_scene(scene),
	m_graphics(g)
{}
void SceneSerializer::Serialize(const char* filepath)
{
	nlohmann::json j;
	j["Scene Name"] = m_scene.GetName();
	DeviceResources* res = m_scene.m_graphics.GetDeviceResources();
	j["device"] = SerializeDevice(*res);
	j["graphics"] = SerializeGraphics(m_scene.m_graphics);
	j["window"] = SerializeWindow( m_scene.m_win);
	j["SceneCamera"] = SerializeSceneCamera( m_scene.sceneCamera);
	j["SceneShader"] = SerializeSceneShader( m_scene.shaders);

	
	j["model"] = nlohmann::json::array();
	for (auto& object : m_scene.objects)
	{
//		j["objects"].push_back(SerializeSceneModels(object));
	}
	
	// Write to file
	std::ofstream file(filepath);
	if (file.is_open()) 
	{
		file << j.dump(4); // Pretty print with 4 spaces
		file.close();
	}
}

void SceneSerializer::Deserialize(const char* filepath)
{
	std::ifstream file(filepath);
	nlohmann::json j;
	file >> j;

	//if (!j["Scene Name"])
	//	MessageBox(nullptr, (LPCWSTR)filepath, L"file does not contain a scene", MB_OK);

	m_scene.SetName(j["Scene Name"]);
	DeviceResources* res = m_scene.m_graphics.GetDeviceResources();
	DeserializeDevice(*res, j["device"]);
	DeserializeGraphics(m_scene.m_graphics, j["graphics"]);
	DeserializeWindow(m_scene.m_win, j["window"]);
	DeserializeSceneCamera(m_scene.sceneCamera, j["SceneCamera"]);
	//DeserializeSceneShader(m_scene.defaultShader, j["defaultShader"]);
	//DeserializeSceneShader(m_scene.texturedShader, j["texturedShader"]);

	// Clear existing models
//	m_scene.m_models.clear();

	// Deserialize models
	for (const auto& model_json : j["model"])
	{
		Model* model = new Model("", m_graphics, m_scene.shaders[1]);
		DeserializeSceneModels(model, model_json);
//		m_scene.m_models.push_back(model);
	}
}


nlohmann::json SceneSerializer::SerializeDevice(const DeviceResources& device)
{
	nlohmann::json j;

	j["width"] = device.GetWidth();
	j["height"] = device.GetHeight();
	j["isFullscreenEnabled"] = device.isFullscreenEnabled;
	j["isVsyncEnabled"] = device.isVsyncEnabled;
	j["enableMsaa"] = device.enableMsaa;

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
	j["Window Title"] = win.m_windowTitle;
	j["Window Class Name"] = win.m_windowClass;
	j["width"] = win.m_width;
	j["height"] = win.m_height;
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

	//j["camYaw"] = camera->GetYaw();
	//j["camPitch"] = camera->GetPitch();
	//j["camRoll"] = camera->GetCamRoll();
	//
	//j["moveLeftRight"] = camera->GetMoveLeftRight();
	//j["moveBackForward"] = camera->GetMoveBackForward();
	//j["moveUpDown"] = camera->GetMoveUpDown();
	//
	//j["cameraSpeed"] = camera->GetCameraSpeed();
	//j["distance"] = camera->distance;

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


nlohmann::json SceneSerializer::SerializeSceneModels(Model* model)
{
	nlohmann::json j;
	j["name"] = model->m_name;
	j["isTextured"] = model->isTextured;
	XMFLOAT3 position = XMFLOAT3(Math::XMVectorToFloat3(model->GetTranslation()));
	XMFLOAT3 scale = XMFLOAT3(Math::XMVectorToFloat3(model->GetScale()));
	XMFLOAT4 rotation = XMFLOAT4(Math::XMVectorToFloat4(model->GetRotation()));
	j["position"] = { position.x,position.y,position.z };
	j["scale"] = { scale.x,scale.y,scale.z };
	j["rotation"] = { rotation.x,rotation.y,rotation.z,rotation.w };

	//if (model->isTextured && model->texture)
	//{
	//	j["texture path"] = model->m_path;
	//	j["textureSlot"] = model->getSlot();
	//}
	//mesh
	if (model->getMesh())
	{
		j["mesh"] = SerializeMeshParts(model->part.get());
	}
	return j;
}

nlohmann::json SceneSerializer::SerializeMeshParts(MeshParts* part)
{
	nlohmann::json j;
	j["indices"] = part->m_indices;
	j["vertices"] = nlohmann::json::array();
	for (auto& vertex : part->m_vertices)
	{
		j["vertices"].push_back(SerializeVertex(&vertex));
	}
	return j;
}

nlohmann::json SceneSerializer::SerializeVertex(Vertex* v)
{
	nlohmann::json j;
	
	switch (v->type) {
	case VertexType::Pos3Col:
		j = nlohmann::json{
			{"pos", {v->pos3Col.pos.x, v->pos3Col.pos.y, v->pos3Col.pos.z}},
			{"col", {v->pos3Col.col.x, v->pos3Col.col.y, v->pos3Col.col.z, v->pos3Col.col.w}}
		};
		break;
	case VertexType::Pos3Tex:
		j = nlohmann::json{
			{"pos", {v->pos3Tex.pos.x, v->pos3Tex.pos.y, v->pos3Tex.pos.z}},
			{"tex", {v->pos3Tex.tex.x, v->pos3Tex.tex.y}}
		};
		break;
	case VertexType::Pos3TexNorm:
		j = nlohmann::json{
			{"pos", {v->pos3TexNorm.pos.x, v->pos3TexNorm.pos.y, v->pos3TexNorm.pos.z}},
			{"tex", {v->pos3TexNorm.tex.x, v->pos3TexNorm.tex.y}},
			{"norm",{v->pos3TexNorm.norm.x,v->pos3TexNorm.norm.y,v->pos3TexNorm.norm.z}}
		};
		break;
	}
	j["type"] = static_cast<int>(v->type);

	return j;
}

void SceneSerializer::DeserializeDevice(DeviceResources& device, const nlohmann::json& j)
{
	device.m_height = (j["height"]);
	device.isFullscreenEnabled = j["isFullscreenEnabled"];
	device.m_width = (j["width"]);
	device.enableMsaa = j["enableMsaa"];
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
	//win.m_windowTitle = j["Window Title"].get<std::wstring>().c_str();
	//win.m_windowClass = j["Window Class Name"].get<std::wstring>().c_str();
	win.m_width = j["width"];
	win.m_height = j["height"];
}

void SceneSerializer::DeserializePerspectiveCamera(PerspectiveCamera* camera, const nlohmann::json& j)
{

	//camera->SetCamera(j["FOV"], j["aspectRatio"],j["nearPlane"], j["farPlane"]);

	DirectX::XMFLOAT3 position(j["position"][0], j["position"][1], j["position"][2]);
	camera->SetPosition(DirectX::XMLoadFloat3(&position));

	DirectX::XMFLOAT3 target(j["target"][0], j["target"][1], j["target"][2]);
	camera->SetTarget(DirectX::XMLoadFloat3(&target));

	DirectX::XMFLOAT3 up(j["up"][0], j["up"][1], j["up"][2]);
	camera->SetUP(DirectX::XMLoadFloat3(&up));

	//camera->setYaw(j["camYaw"]);
	//camera->SetPitch(j["camPitch"]);
	//camera->SetRoll(j["camRoll"]);
	//
	//camera->SetMoveLeftRight(j["moveLeftRight"]);
	//camera->SetMoveBackForward(j["moveBackForward"]);
	//camera->SetMoveUpDown(j["moveUpDown"]);
	//
	//camera->cameraSpeed = (j["cameraSpeed"]);
	//camera->distance = j["distance"];

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
			PerspectiveCamera* perspectiveCamera = new PerspectiveCamera();
			DeserializePerspectiveCamera(perspectiveCamera, cameraObj["Perspective Camera"]);
			//cam->SetPerspectiveCamera(perspectiveCamera);
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

void SceneSerializer::DeserializeSceneModels(Model* model, const nlohmann::json& j)
{
	model->m_name = j["name"];
	model->isTextured = j["isTextured"];

	XMFLOAT3 position(j["position"][0], j["position"][1], j["position"][2]);
	model->setTranslation(position);

	XMFLOAT3 scale(j["scale"][0], j["scale"][1], j["scale"][2]);
	model->setScale(scale);

	XMFLOAT4 rotation(j["rotation"][0], j["rotation"][1], j["rotation"][2], j["rotation"][3]);
	model->setRotation(rotation);



	if (j.contains("mesh"))
	{
		model->part = std::make_unique<MeshParts>(m_graphics);
		DeserializeMeshParts(model->part.get(), j["mesh"]);

		if (model->isTextured)
		{
			//model->TexturedMesh(model->part->getVertices(), model->part->getIndices(), j["texture path"].get<std::string>().c_str(), j["textureSlot"]);
		}
		model->CreateMesh(model->part->getVertices(), model->part->getIndices());
		
	}

}

void SceneSerializer::DeserializeMeshParts(MeshParts* part, const nlohmann::json& j)
{
	part->m_indices = j["indices"].get<std::vector<unsigned short>>();
	for (const auto& vertex_json : j["vertices"])
	{
		Vertex vertex;
		DeserializeVertex(&vertex, vertex_json);
		part->m_vertices.push_back(vertex);
	}
	
}

void SceneSerializer::DeserializeVertex(Vertex* v, const nlohmann::json& j)
{
	v->type = j["type"];
	switch (v->type) {
	default:
	case VertexType::Pos3Col:
		v->pos3Col.pos = DirectX::XMFLOAT3(j["pos"][0], j["pos"][1], j["pos"][2]);
		v->pos3Col.col = DirectX::XMFLOAT4(j["col"][0], j["col"][1], j["col"][2], j["col"][3]);
		break;
	case VertexType::Pos3Tex:
		v->pos3Tex.pos = DirectX::XMFLOAT3(j["pos"][0], j["pos"][1], j["pos"][2]);
		v->pos3Tex.tex = DirectX::XMFLOAT2(j["tex"][0], j["tex"][1]);
		break;
	case VertexType::Pos3TexNorm:
		v->pos3TexNorm.pos = DirectX::XMFLOAT3(j["pos"][0], j["pos"][1], j["pos"][2]);
		v->pos3TexNorm.tex = DirectX::XMFLOAT2(j["tex"][0], j["tex"][1]);
		v->pos3TexNorm.norm = DirectX::XMFLOAT3(j["norm"][0], j["norm"][1], j["norm"][2]);
		break;
	}

}
