#pragma once
#include "Scene\RenderableObject.h"
#include "Mesh.h"


class Model : public RenderableObject
{
public:
	Model(const std::string& name,Graphics& g, ShaderManager shaderManager);

	void CreateMesh( std::vector<unsigned short>& indices, std::vector<Vertex> vertices);
	virtual void Render() override;
	void controlWindow()override ;

	

private:
protected:
	Graphics& m_graphics;
	Mesh m_mesh;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_orientation;
	DirectX::XMVECTOR m_scale;
};

