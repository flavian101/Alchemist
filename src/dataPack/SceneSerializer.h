#pragma once
#include "Scene/SceneManager.h"
#include "Json.hpp"

class SceneSerializer
{
public:
	SceneSerializer(const Scene& scene);
	void Serialize(const char* filepath);
	void Deserialize(const char* filepath);
private:
	nlohmann::json SerializeGraphics(Graphics& graphics);

	nlohmann::json SerializeWindow(Window& win);

	nlohmann::json SerializePerspectiveCamera(PerspectiveCamera* camera);

	nlohmann::json SerializeOrthographicCamera(OrthographicCamera* camera);

	nlohmann::json SerializeSceneCamera(SceneCamera* camera);

	nlohmann::json SerializeDefaultShader(std::shared_ptr<ShaderManager> shader);

	nlohmann::json SerializeTexturedShader(std::shared_ptr<ShaderManager> shader);

	nlohmann::json SerializeSceneModels(Model* model);

	nlohmann::json SerializeMeshParts(MeshParts* part);

	nlohmann::json SerializeVertex(Vertex* v);
	


private:
	const Scene& m_scene;
	//friend Scene;
};

