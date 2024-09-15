#pragma once
#include "Scene/SceneManager.h"
#include "Json.hpp"

class Renderer;
class Graphics;
class Window;
class Scene;
class PerspectiveCamera;
class OrthographicCamera;
class SceneCamera;
class ShaderManager;
class Model;
class Mesh;
class Vertex;


class SceneSerializer
{
public:
	SceneSerializer( Scene& scene,Graphics& g);
	void Serialize(const char* filepath);
	void Deserialize(const char* filepath);
private:
	//Serialize
	nlohmann::json SerializeDevice(const Renderer& device);
	
	nlohmann::json SerializeGraphics(Graphics& graphics);

	nlohmann::json SerializeWindow(Window& win);

	nlohmann::json SerializePerspectiveCamera(PerspectiveCamera* camera);

	nlohmann::json SerializeOrthographicCamera(OrthographicCamera* camera);

	nlohmann::json SerializeSceneCamera(SceneCamera* camera);

	nlohmann::json SerializeSceneShader(std::vector<std::shared_ptr<ShaderManager>> shader);

	nlohmann::json SerializeSceneModels(Model* model);

//	nlohmann::json SerializeMeshParts(MeshParts* part);

	nlohmann::json SerializeVertex(Vertex* v);


	//Deserialize 
	void DeserializeGraphics(Graphics& graphics, const nlohmann::json& j);

	void DeserializeDevice(Renderer& device, const nlohmann::json& j);

	void DeserializeWindow(Window& win, const nlohmann::json& j);

	void DeserializePerspectiveCamera(PerspectiveCamera* camera, const nlohmann::json& j);

	void DeserializeOrthographicCamera(OrthographicCamera* camera, const nlohmann::json& j);

	void DeserializeSceneCamera(SceneCamera* camera, const nlohmann::json& j);

	void DeserializeSceneShader(std::shared_ptr<ShaderManager> shader, const nlohmann::json& j);

	void DeserializeSceneModels(Model* model, const nlohmann::json& j);

	//void DeserializeMeshParts(MeshParts* part, const nlohmann::json& j);

	void DeserializeVertex(Vertex* v, const nlohmann::json& j);
	
	


private:
	Scene& m_scene;
	Graphics& m_graphics;
	//friend Scene;
};

