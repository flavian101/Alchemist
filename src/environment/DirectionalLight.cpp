#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager)
	:
	Light(name, g, manager)
{

}
