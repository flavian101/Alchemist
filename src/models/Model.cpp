#include "Model.h"
Model::Model(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> shaderManager)
	:
	RenderableObject(name,g,std::move(shaderManager)),
	m_graphics(g),
	m_manager(std:: move(shaderManager)),
	m_mesh(g),
	part(nullptr),
	samp(nullptr),
	isTextured(false),
	yaw(0.0f),
	pitch(0.0f)
{
	material =std::make_unique< Material>(m_graphics);
}

Model::~Model()
{
	delete part;
	delete samp;
}


void Model::TexturedMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices, 
	const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* aoPath)
{
	isTextured = true;
	samp = new Utils::Sampler(m_graphics);
	samp->Bind();
	albedoTexture = std::make_unique<Utils::Texture>(m_graphics);
	normalTexture = std::make_unique<Utils::Texture>(m_graphics);
	metallicTexture = std::make_unique<Utils::Texture>(m_graphics);
	roughnessTexture = std::make_unique<Utils::Texture>(m_graphics);
	aoTexture = std::make_unique<Utils::Texture>(m_graphics);

	albedoTexture->LoadTexture(albedoPath, 0);
	normalTexture->LoadTexture(normalPath, 1);
	metallicTexture->LoadTexture(metallicPath, 2);
	roughnessTexture->LoadTexture(roughnessPath, 3);
	aoTexture->LoadTexture(aoPath, 4);
	CreateMesh(vertices, indices);

}

void Model::CreateMesh( const std::vector<Vertex>& vertices,const std::vector<unsigned short>& indices )
{
	part = new MeshParts(m_graphics);
	part->Initialize(indices, vertices);
	m_mesh.AddMeshPart(*part);
}

MeshParts* Model::getMesh()
{
	return part;
}

void Model::Update(float deltaTime)
{
	material->Update();
	RenderableObject::Update(deltaTime);
}

void Model::Render()
{
	material->Bind();
	if (isTextured)
	{
		albedoTexture->Bind();
		normalTexture->Bind();
		metallicTexture->Bind();
		roughnessTexture->Bind();
		aoTexture->Bind();
	}
	RenderableObject::Render();
	m_mesh.Bind();
	m_mesh.Render();
}

void Model::controlWindow()
{
	RenderableObject::controlWindow();
	ImGui::Separator();
	material->controlWindow();
	ImGui::Text("Information");
	ImGui::Text("name: %s",m_name.c_str());
	ImGui::Text("index count: %d", part->getIndices().size());
	ImGui::Text("Vertex count: %d", part->getVertices().size());
}

void Model::Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime)
{
	m_position += direction * speed * deltaTime;
	setTranslation(Math::XMVectorToFloat3(m_position));
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
	setRotation(Math::XMVectorToFloat4(m_orientation));
}





