#include "Node.h"
#include "Mesh.h"

Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in)
	:
	id(id),
	m_name(name),
	meshPtrs(std::move(meshPtrs))
{
	XMStoreFloat4x4(&transform, transform_in);
	XMStoreFloat4x4(&appliedTransform, XMMatrixIdentity());
}

void Node::Update(Graphics& gfx, float deltaTime)
{

}

void Node::Draw(Graphics& gfx, FXMMATRIX accumulatedTransform)
{
	const auto built =
		XMLoadFloat4x4(&appliedTransform) *
		XMLoadFloat4x4(&transform) *
		accumulatedTransform;

	for (const auto pm : meshPtrs)
	{
		pm->Update(gfx);
		pm->Bind(gfx);
		pm->Render(gfx, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(gfx, built);
	}
}

void Node::SetAppliedTransfrom(FXMMATRIX transform)
{
	XMStoreFloat4x4(&appliedTransform, transform);
}

int Node::getID() const
{
	return 0;
}

void Node::ShowTree(Node*& pSelectedNode) const
{
}

void Node::ControlNode(Graphics& gfx)
{
	//material control
}

void Node::AddChild(std::unique_ptr<Node> child)
{
	assert(child);
	childPtrs.push_back(std::move(child));
}
