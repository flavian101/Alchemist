#pragma once
#include "Scene\RenderableObject.h"
#include "Mesh.h"
#include "Graphics/Camera/ThirdPerson.h"
#include <algorithm>
#include "Material.h"

class Model : public RenderableObject
{
public:
	Model(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> shaderManager);
	~Model();

	void TexturedMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices, 
		const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* aoPath);
	void CreateMesh(const std::vector<Vertex>& vertices,const  std::vector<unsigned short>& indices );
	MeshParts* getMesh();
	UINT getSlot() { return m_slot; }
	virtual void Update(float deltaTime)override;
	virtual void Render() override;
	void controlWindow()override ;

	void Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime);
	void Rotate(float yawAmount, float pitchAmount);

	
private:
	friend class SceneSerializer;

protected:
	Graphics& m_graphics;
	Mesh m_mesh;
	MeshParts* part;
	std::shared_ptr<ShaderManager> m_manager;
	Utils::Sampler* samp;
	std::unique_ptr<Utils::Texture> albedoTexture;
	std::unique_ptr<Utils::Texture> normalTexture;
	std::unique_ptr<Utils::Texture> metallicTexture;
	std::unique_ptr<Utils::Texture> roughnessTexture;
	std::unique_ptr<Utils::Texture> aoTexture;
	UINT m_slot;
	bool isTextured;
	const char* m_albedoPath;
	const char* m_normalPath;
	const char* m_metallicPath;
	const char* m_roughnessPath;
	const char* m_aoPath;
	std::unique_ptr<Material> material;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_orientation;
	DirectX::XMVECTOR m_scale;
	float yaw;
	float pitch;
};

