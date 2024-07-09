#include "Model.h"
Model::Model(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> shaderManager)
	:
	RenderableObject(name,g,std::move(shaderManager)),
	m_graphics(g),
	m_manager(std:: move(shaderManager)),
	m_mesh(g),
	part(nullptr),
	isTextured(false),
	yaw(0.0f),
	pitch(0.0f)
{
	material =std::make_unique< Material>(m_graphics);
}

Model::~Model()
{
	delete part;
}


void Model::TexturedMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices, 
	const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* aoPath)
{
	isTextured = true;
	if (albedoPath) material->LoadTexture(Material::TextureType::Albedo, albedoPath);
	if (normalPath) material->LoadTexture(Material::TextureType::Normal, normalPath);
	if (metallicPath) material->LoadTexture(Material::TextureType::Metallic, metallicPath);
	if (roughnessPath) material->LoadTexture(Material::TextureType::Roughness, roughnessPath);
	if (aoPath) material->LoadTexture(Material::TextureType::AmbientOcclusion, aoPath);
	m_mesh.SetMaterial(std::move(material));
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
	RenderableObject::Update(deltaTime);
}

void Model::Render()
{
	RenderableObject::Render();
	m_mesh.Bind();
	m_mesh.Render();
}

void Model::controlWindow()
{
	RenderableObject::controlWindow();
	m_mesh.controlWindow();
	ImGui::Separator();
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





