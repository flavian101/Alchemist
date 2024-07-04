#pragma once
#include "Light.h"
#include "Graphics/Graphics.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);
	


};

