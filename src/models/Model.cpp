#include "Model.h"
#include "Graphics/Camera/ThirdPerson.h"
#include "Material.h"


Model::Model(const std::string& name)
:
	GameObject(name),
	pWindow(std::make_unique<ModelWindow>())
{}

Model::~Model()
{
	//m_meshes.clear();
}

void Model::Update(Graphics& gfx,float deltaTime)
{
	GameObject::Update(gfx, deltaTime);
	pRoot->Update(gfx,deltaTime);
}

void Model::SetRootTransform(FXMMATRIX tf)
{
	pRoot->SetAppliedTransfrom(tf);
}

void Model::Render(Graphics& gfx)
{
	GameObject::Render(gfx);
	if (auto node = pWindow->getSelectedNode())
	{
		node->SetAppliedTransfrom(pWindow->GetTransform());
	}
	pRoot->Draw(gfx, XMMatrixIdentity());
}

void Model::controlWindow(Graphics& gfx)
{
	GameObject::controlWindow(gfx);
	pWindow->show(gfx, *pRoot);
	ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.4f, 0.5f), "information");

}
