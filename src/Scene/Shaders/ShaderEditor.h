#pragma once
#include <string>
#include <memory>

class ShaderManager;
class Graphics;

class ShaderEditor
{
public:
	ShaderEditor(std::shared_ptr<ShaderManager> shader);

	void Render(Graphics& gfx);


private:
	std::shared_ptr<ShaderManager> m_shaderManager;
	std::string m_vertexShaderCode;
	std::string m_pixelShaderCode;
	bool m_needsReload;
	bool m_needsSaving;
};

