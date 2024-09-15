#pragma once
#include <algorithm>
#include "Scene\RenderableObject.h"
#include <vector>
#include <string>


class Graphics;
class Mesh;
class ThirdPerson;
class Material;
class Vertex;

class Model : public RenderableObject
{
public:
	Model(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> shaderManager);
	~Model();

	void AddMesh(Mesh* mesh);

	virtual void Update(float deltaTime)override;
	virtual void Render() override;
	void controlWindow()override ;

	void Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime);
	void Rotate(float yawAmount, float pitchAmount);

	
private:
	friend class SceneSerializer;

protected:
	Graphics& m_graphics;
	std::vector<Mesh*> m_meshes;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_orientation;
	DirectX::XMVECTOR m_scale;
	float yaw;
	float pitch;
};

