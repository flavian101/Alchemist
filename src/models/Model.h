#pragma once
#include "Scene\RenderableObject.h"
#include "Mesh.h"


class Model : public RenderableObject
{
public:
	Model(const std::string& name,Graphics& g, ShaderManager shaderManager);
	~Model();

	void TexturedMesh(const std::vector<Vertex>& vertices, const  std::vector<unsigned short>& indices, const char* path, UINT slot);
	void CreateMesh(const std::vector<Vertex>& vertices,const  std::vector<unsigned short>& indices );

	virtual void Render() override;
	void controlWindow()override ;

	

private:
protected:
	Graphics& m_graphics;
	Mesh m_mesh;
	Utils::Sampler* samp;
	Utils::Texture* texture;
	bool isTextured;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_orientation;
	DirectX::XMVECTOR m_scale;
};

