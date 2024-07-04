#include "PointLight.h"

PointLight::PointLight(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager)
	:
	Light(name, g, manager)
{
}

void PointLight::SetRange(float range)
{
}

void PointLight::Update(float deltaTime)
{
}

void PointLight::Render()
{
}

void PointLight::controlWindow()
{
}
