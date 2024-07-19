#pragma once
#include "Light.h"

class Graphics;

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);
	


};

