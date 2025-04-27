#pragma once
#include "Scene/ClientSceneManager.h"
#include "Json.hpp"

class Renderer;
class Graphics;
class Window;
class Scene;
class PerspectiveCamera;
class FreeLook;
class ThirdPerson;
class FirstPerson;
class OrthographicCamera;
class SceneCamera;
class ShaderManager;
class Model;
class Node;
class Mesh;
class Vertex;


class SceneSerializer
{
public:
	SceneSerializer(Graphics& g);
	nlohmann::json Serialize(Scene* scene, const std::string& projectDir, const std::string& projectName);
	void Deserialize(Scene* scene, nlohmann::json j);
private:
	//Serialize
	nlohmann::json SerializeDevice(const Renderer& device);
	
	nlohmann::json SerializeGraphics(Graphics& graphics);

	nlohmann::json SerializeWindow(Window& win);

	nlohmann::json SerializePerspectiveCamera(PerspectiveCamera* camera);

	nlohmann::json SerializeFreelook(FreeLook* look);

	nlohmann::json SerializeThirdPerson(ThirdPerson* camera);

	nlohmann::json SerializeFirstPerson(FirstPerson* camera);

	nlohmann::json SerializeOrthographicCamera(OrthographicCamera* camera);

	nlohmann::json SerializeSceneCamera(SceneCamera* camera);

	nlohmann::json SerializeSceneShader(std::vector<std::shared_ptr<ShaderManager>> shader);

	nlohmann::json SerializeModel(Model* model);

	nlohmann::json SerializeNode(Node& node);


	//Deserialize 
	void DeserializeGraphics(Graphics& graphics, const nlohmann::json& j);

	void DeserializeDevice(Renderer& device, const nlohmann::json& j);

	void DeserializeWindow(Window& win, const nlohmann::json& j);

	void DeserializePerspectiveCamera(PerspectiveCamera* camera, const nlohmann::json& j);

	void DeserializeFreelook(FreeLook* look, const nlohmann::json& j);

	void DeserializeThirdPerson(ThirdPerson* camera, const nlohmann::json& j);

	void DeserializeFirstPerson(FirstPerson* camera, const nlohmann::json& j);

	void DeserializeOrthographicCamera(OrthographicCamera* camera, const nlohmann::json& j);

	void DeserializeSceneCamera(SceneCamera* camera, const nlohmann::json& j);

	void DeserializeSceneShader(std::shared_ptr<ShaderManager> shader, const nlohmann::json& j);

	void DeserializeModel(Model* model, const nlohmann::json& j);
	void DeserializeNode(Node& node, const nlohmann::json& j);
	
	


private:
	friend class ClientProject;
	Graphics& m_graphics;
	//friend Scene;
};

