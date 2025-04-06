#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <stdafx.h>
#include <memory>
#include <unordered_map>
#include "Scene/GameObject.h"
#include "Node.h"
#include "Mesh.h"
#include "models/Vertex.h"


class Graphics;
class ThirdPerson;
class Material;


class ModelWindow
{
public:
	void show(Graphics& gfx, const Node& root)
	{
		int nodeIndexTracker = 0;
		ImGui::Columns(2, nullptr, true);
		root.ShowTree(pSelectedNode);

		ImGui::NextColumn();
		if (pSelectedNode != nullptr)
		{
			auto& transform = transforms[pSelectedNode->getID()];
			ImGui::Text("Orientation");
			ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
			ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
			ImGui::Text("Position");
			ImGui::DragFloat("X", &transform.x, 0.1f, 20.0f);
			ImGui::DragFloat("Y", &transform.y, 0.1f, 20.0f);
			ImGui::DragFloat("Z", &transform.z, 0.1f, 20.0f);
			ImGui::Text("Scaling");
			ImGui::DragFloat("scaleX", &transform.scaleX, 0.1f, 1.0f);
			ImGui::DragFloat("scaleY", &transform.scaleY, 0.1f, 1.0f);
			ImGui::DragFloat("scaleZ", &transform.scaleZ, 0.1f, 1.0f);
			//control the node method
		}
	}

	XMMATRIX GetTransform()const
	{

		assert(pSelectedNode != nullptr);
		const auto& transform = transforms.at(pSelectedNode->getID());
		return
			XMMatrixScaling(transform.scaleX,transform.scaleY,transform.scaleZ)*
			XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			XMMatrixTranslation(transform.x, transform.y, transform.z);
	}
	Node* getSelectedNode()const
	{
		return pSelectedNode;
	}
private:
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float scaleX = 1.0f,scaleY= 1.0f,scaleZ = 1.0f;
	};
	std::unordered_map<int, TransformParameters> transforms;

};

class Model : public GameObject
{
public:
	Model(const std::string& name);
	~Model();
	std::vector < std::shared_ptr<Mesh>> getMeshParts()
	{
		return meshPtrs;
	}

	void Update(Graphics& gfx,float deltaTime) override;
	void SetRootTransform(FXMMATRIX tf);
	void Render(Graphics& gfx) override;
	void controlWindow(Graphics& gfx);


	
private:
	friend class SceneSerializer;

protected:
	std::unique_ptr<Node> pRoot;
	std::vector<std::shared_ptr<Mesh>> meshPtrs;
	std::unique_ptr<ModelWindow> pWindow;
};


