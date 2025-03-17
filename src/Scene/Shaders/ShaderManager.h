#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "stdafx.h"

class Graphics;
class Shaders;
//class ShaderEditor;

class ShaderManager
{
public:
	ShaderManager(Graphics& gfx);
	~ShaderManager();
	void BindShader(Graphics& gfx,const char* shaderName);
	void UnBindShader(Graphics& gfx,const char* shaderName);


	
private:
	//std::unique_ptr<ShaderEditor> shaderEditor;
	std::unordered_map<std::string,std::unique_ptr<Shaders>> shaders;
	
};

