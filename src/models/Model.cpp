#include "Model.h"
#include "Graphics/Camera/ThirdPerson.h"
#include "Material.h"
#include "Mesh.h"
#include "models/Vertex.h"


Model::Model(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> shaderManager)
	:
	RenderableObject(name,g,shaderManager),
	m_graphics(g),
	yaw(0.0f),
	pitch(0.0f)
{
}

Model::~Model()
{
	//m_meshes.clear();
}
void Model::AddMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
}

void Model::Update(float deltaTime)
{
	RenderableObject::Update(deltaTime);
	for (auto& mesh : m_meshes)
	{
		mesh->Update(deltaTime);
	}
	
}

void Model::Render()
{
	RenderableObject::Render();
	for (auto& mesh : m_meshes)
	{
		mesh->Bind();
		mesh->Render();
	}
}

void Model::controlWindow()
{
	RenderableObject::controlWindow();
	for (auto& mesh : m_meshes)
	{
		mesh->controlWindow();
	}
	ImGui::Separator();
	ImGui::Text("Information");
	ImGui::Text("name: %s",m_name.c_str());
	//ImGui::Text("Index Count: %d", static_cast<int>(m_mesh->getIndexCount()));
	//ImGui::Text("Vertex Count: %d", static_cast<int>(m_mesh->getVertexCount()));

	ImGui::Text("Mesh");
	
}

void Model::Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime)
{
	m_position += direction * speed * deltaTime;
	//setTranslation(Math::XMVectorToFloat3(m_position));
}

void Model::Rotate(float yawAmount,float pitchAmount)
{
	yaw += yawAmount;
	pitch += pitchAmount;
	// Clamp pitch to avoid gimbal lock
	//pitch = std::max(-XM_PIDIV2 + 0.1f, std::min(XM_PIDIV2 - 0.1f, pitch));

	// Create a quaternion based on the yaw and pitch
	DirectX::XMVECTOR yawQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), yaw);
	DirectX::XMVECTOR pitchQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), pitch);

	// Combine the yaw and pitch quaternions
	m_orientation = DirectX::XMQuaternionMultiply(pitchQuat, yawQuat);
	m_orientation = DirectX::XMQuaternionNormalize(m_orientation);
	//setRotation(Math::XMVectorToFloat4(m_orientation));
}





