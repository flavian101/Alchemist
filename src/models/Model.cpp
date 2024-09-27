#include "Model.h"
#include "Graphics/Camera/ThirdPerson.h"
#include "Material.h"
#include "Mesh.h"
#include "Node.h"
#include "models/Vertex.h"
#include "Scene/Shaders/ShaderManager.h"


Model::Model(const std::string& name, std::shared_ptr<ShaderManager> shaderManager)
{
}

Model::~Model()
{
	//m_meshes.clear();
}


void Model::Update(Graphics& gfx,float deltaTime)
{
	
	pRoot->Update(gfx,deltaTime);
	
	
}

void Model::SetRootTransform(FXMMATRIX tf)
{
	pRoot->SetAppliedTransfrom(tf);
}

void Model::Render(Graphics& gfx)
{
	pRoot->Draw(gfx, XMMatrixIdentity());
	
}

void Model::controlWindow()
{

}
