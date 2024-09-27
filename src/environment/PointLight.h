#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);
	void SetRange(float range);

	void Update(Graphics& gfx,float deltaTime)override;
	void Render(Graphics& gfx)override;
	void controlWindow()override;

};

