#include "SceneSerializer.h"
#include <fstream>


SceneSerializer::SceneSerializer(const Scene& scene)
	:
	m_scene(scene)
{
}

nlohmann::json SceneSerializer::SerializeGraphics(Graphics& graphics)
{
	nlohmann::json j;

	j["width"] = graphics.getWidth();
	j["height"] = graphics.getHeight();
	j["isFullscreenEnabled"] = graphics.isFullscreenEnabled;
	j["isVsyncEnabled"] = graphics.isVsyncEnabled;
	j["enableMsaa"] = graphics.enableMsaa;
	j["imguiEnabled"] = graphics.isImguiEnabled();
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

nlohmann::json SceneSerializer::SerializeWindow( Window& win)
{
	nlohmann::json j;
	j["Window Title"] = win.m_windowTitle;
	j["Window Class Name"] = win.m_windowClass;
	j["width"] = win.m_width;
	j["height"] =win.m_height;
	return j;
}

nlohmann::json SceneSerializer::SerializePerspectiveCamera( PerspectiveCamera* camera)
{
	nlohmann::json j;
	j["FOV"] = camera->GetFOV();
	j["aspectRatio"] = 0;
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

	j["camYaw"] = camera->GetYaw();
	j["camPitch"] = camera->GetPitch();
	j["camRoll"] = camera->GetCamRoll();

	j["moveLeftRight"] = camera->GetMoveLeftRight();
	j["moveBackForward"] = camera->GetMoveBackForward();
	j["moveUpDown"] = camera->GetMoveUpDown();

	j["cameraSpeed"] = camera->GetCameraSpeed();
	j["distance"] = camera->distance;

	return j;
}

nlohmann::json SceneSerializer::SerializeOrthographicCamera( OrthographicCamera* camera)
{
	nlohmann::json j;
	return j;
}

nlohmann::json SceneSerializer::SerializeSceneCamera( SceneCamera* camera)
{
	nlohmann::json j;
	j["Camera Name"] = camera->m_name;
	j["IsPerspective"] = camera->isPerspective;
	if (camera->isPerspective)
	{
		j["Perspective Camera"] = SerializePerspectiveCamera(camera->GetPerspective());
	}
	else
	{
		j["Orthographic Camera"] = SerializeOrthographicCamera(camera->GetOrthographic());

	}
	j["showCreateWindow"] = camera->showCreateWindow;
	return j;
}

nlohmann::json SceneSerializer::SerializeDefaultShader(std::shared_ptr<ShaderManager> shader)
{
	nlohmann::json j;
	return nlohmann::json();
}

nlohmann::json SceneSerializer::SerializeTexturedShader( std::shared_ptr<ShaderManager> shader)
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

	if (model->isTextured && model->texture)
	{
		j["textureSlot"] = model->getSlot();
	}
	//mesh
	if (model->getMesh())
	{
		j["mesh"] = SerializeMeshParts(model->part);
	}
	return j;
}

nlohmann::json SceneSerializer::SerializeMeshParts( MeshParts* part)
{
	nlohmann::json j;
	j["indices"] = part->m_indices;
	j["vertices"] = nlohmann::json::array();
	for ( auto& vertex : part->m_vertices)
	{
		j["vertices"].push_back(SerializeVertex(&vertex));
	}
	return j;
}

nlohmann::json SceneSerializer::SerializeVertex(Vertex* v)
{
	nlohmann::json j;
	switch (v->type) {
	default:
		j["type"] = v->type;
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
	return j;
}

void SceneSerializer::Serialize(const char* filepath)
{
	nlohmann::json j;
	j["Scene Name"] = m_scene.GetName();
	j["graphics"] = SerializeGraphics(m_scene.m_graphics);
	j["window"] = SerializeWindow( m_scene.m_win);
	j["SceneCamera"] = SerializeSceneCamera( m_scene.sceneCamera);
	j["defaultShader"] = SerializeDefaultShader( m_scene.defaultShader);
	j["texturedShader"] = SerializeTexturedShader( m_scene.texturedShader);
	
	j["model"] = nlohmann::json::array();
	for (auto& model : m_scene.m_models)
	{
		j["model"].push_back(SerializeSceneModels(model));
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
}
