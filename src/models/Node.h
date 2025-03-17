#pragma once
#include <memory>
#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>

class Graphics;

class Mesh;

static int coppy = 0;
static int move = 0;
class Node
{
public:
	Node(int id,const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in);
	Node(int id,const std::string& name,const DirectX::FXMMATRIX& transform_in);

	//Node(Node&& other)
	//	: id(other.id), m_name(other.m_name), childPtrs(other.childPtrs),
	//	meshPtrs(other.meshPtrs), transform(other.transform), appliedTransform(other.appliedTransform)
	//{
	//	move++;
	//	std::cout << "moved " << move << std::endl;
	//}

	void Update(Graphics& gfx, float deltaTime);
	void Draw(Graphics& gfx, FXMMATRIX accumulatedTransform);
	void SetAppliedTransfrom(FXMMATRIX transform);
	int getID()const;
	void ShowTree(Node*& pSelectedNode)const;
	void ControlNode(Graphics& gfx);///material buffer
	void InformationWindow();

private:
	void AddChild(std::unique_ptr<Node> child);
	void AddMesh(Mesh* meshPart);

private:
	std::string m_name;
	int id;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;
private:
	friend class ModelLoader;
	friend class ModelBuilder;
};

