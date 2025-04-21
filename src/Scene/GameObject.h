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
	GameObject(const std::string& name);
	~GameObject();

	void SetFilePath(const std::string& path);
	std::string GetFilePath()const;
	virtual void Update(Graphics& gfx,float time);
	virtual void Render(Graphics& gfx);

	//transform
	virtual DirectX::XMMATRIX GetTransform() const;
	virtual void SetTransform(DirectX::FXMMATRIX transform);

	
	//motion
	void Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime);
	void Rotate(float yawAmount, float pitchAmount);

	std::string GetName()const;
	void setName(const std::string& name);
	virtual void controlWindow(Graphics& gfx);
	
private:

	

protected:
	std::string filepath = "";
	std::string m_name = "untitled";
	DirectX::XMFLOAT4X4 m_transform; // Store the transform matrix


};

