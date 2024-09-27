#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <memory>

class Graphics;

class Mesh;

class Node
{
public:
	Node(int id,const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in);
	void Update(Graphics& gfx, float deltaTime);
	void Draw(Graphics& gfx, FXMMATRIX accumulatedTransform);
	void SetAppliedTransfrom(FXMMATRIX transform);
	int getID()const;
	void ShowTree(Node*& pSelectedNode)const;
	void ControlNode(Graphics& gfx);///material buffer

private:
	void AddChild(std::unique_ptr<Node> child);

private:
	std::string m_name;
	int id;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;
private:
	friend class ModelLoader;
};

