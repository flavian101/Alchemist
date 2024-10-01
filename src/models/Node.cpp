#include "Node.h"
#include "Mesh.h"
#include "imgui/imgui.h"

Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in)
    : id(id), m_name(name), meshPtrs(std::move(meshPtrs))
{
    XMStoreFloat4x4(&transform, transform_in);
    XMStoreFloat4x4(&appliedTransform, XMMatrixIdentity());
}

Node::Node(int id, const std::string& name, const DirectX::FXMMATRIX& transform_in)
    : id(id),
    m_name(name)
{
    XMStoreFloat4x4(&transform, transform_in);
    XMStoreFloat4x4(&appliedTransform, XMMatrixIdentity());
}


void Node::Update(Graphics& gfx, float deltaTime)
{
    // Update logic
}

void Node::Draw(Graphics& gfx, FXMMATRIX accumulatedTransform)
{
    const auto built = XMLoadFloat4x4(&appliedTransform) * XMLoadFloat4x4(&transform) * accumulatedTransform;

    for (const auto& pm : meshPtrs)
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
    return id;
}

void Node::ShowTree(Node*& pSelectedNode) const
{
    const int selectedID = (pSelectedNode == nullptr) ? -1 : pSelectedNode->getID();

    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow |
        ((getID() == selectedID ? ImGuiTreeNodeFlags_Selected : 0) |
            ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0));
    const auto expanded = ImGui::TreeNodeEx((void*)(intptr_t)getID(), node_flags, m_name.c_str());
    if (ImGui::IsItemClicked())
    {
        pSelectedNode = const_cast<Node*>(this);
    }

    if (expanded)
    {
        for (const auto& pChild : childPtrs)
        {
            pChild->ShowTree(pSelectedNode);
        }
        ImGui::TreePop();
    }
}

void Node::ControlNode(Graphics& gfx)
{
    // Material control logic
}

void Node::InformationWindow()
{
    
}

void Node::AddChild(std::unique_ptr<Node> child)
{
    assert(child);
    childPtrs.emplace_back(std::move(child));
}

void Node::AddMesh(Mesh* meshPart)
{
    assert(meshPart);
    meshPtrs.emplace_back(std::move(meshPart));
}
