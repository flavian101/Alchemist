#pragma once
#include "Scene\RenderableObject.h"
#include "Mesh.h"


class Model : public RenderableObject
{
public:
	Model(Graphics& g, ShaderManager shaderManager);

	void CreateMesh( std::vector<unsigned short>& indices, std::vector<Vertex> vertices);
	virtual void Render() override;

	

private:
protected:
	Graphics& m_graphics;
	Mesh m_mesh;
};

