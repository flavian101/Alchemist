#pragma once
#include <string>
#include <memory>
#include "MathUtils\MathUtils.h"
#include"stdafx.h"

class Graphics;
class ShaderManager;


/// <summary>
/// set the pixel constant buffer
/// </summary>

class RenderableObject
{
public:
	RenderableObject(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> shaderManager);
	~RenderableObject();
	virtual void Update(float time);
	virtual void Render();

	std::shared_ptr<ShaderManager> GetShadermanager()const;
	void SetShaderManager(std::shared_ptr<ShaderManager> manager);
	std::string getName()const;
	void setName(const std::string& name);
	virtual void controlWindow();
	
private:
	Graphics& m_graphics;
	std::shared_ptr<ShaderManager> m_shaderManager;


protected:
	std::string m_name = "untitled";

};

