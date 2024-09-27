#pragma once
#include <string>
#include <memory>
#include "MathUtils\MathUtils.h"
#include"stdafx.h"

class Graphics;
class ShaderManager;
class Model;
class Transform;


/// <summary>
/// set the pixel constant buffer
/// </summary>

class GameObject
{
public:
	GameObject(const std::string& name,Model* model);
	~GameObject();

	virtual void Update(Graphics& gfx,float time);
	virtual void Render(Graphics& gfx);
	
	//motion
	void Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime);
	void Rotate(float yawAmount, float pitchAmount);

	std::string getName()const;
	void setName(const std::string& name);
	virtual void controlWindow();
	
private:
	std::unique_ptr<Model> m_model;

	float yaw;
	float pitch;


protected:
	std::string m_name = "untitled";

};

