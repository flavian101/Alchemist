#pragma once
#include "Scene\RenderableObject.h"
#include "Mesh.h"


class Model : public RenderableObject
{
public:
	Model(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> shaderManager);
	~Model();

	void TexturedMesh(const std::vector<Vertex>& vertices, const  std::vector<unsigned short>& indices, const char* path, UINT slot);
	void CreateMesh(const std::vector<Vertex>& vertices,const  std::vector<unsigned short>& indices );
	MeshParts* getMesh();
	UINT getSlot() { return m_slot; }

	virtual void Render() override;
	void controlWindow()override ;

	

protected:
	Graphics& m_graphics;
	Mesh m_mesh;
	MeshParts* part;
	std::shared_ptr<ShaderManager> m_manager;
	Utils::Sampler* samp;
	Utils::Texture* texture;
	UINT m_slot;
	bool isTextured;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_orientation;
	DirectX::XMVECTOR m_scale;
};

