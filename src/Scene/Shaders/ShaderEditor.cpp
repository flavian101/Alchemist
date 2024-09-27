#include "ShaderEditor.h"
#include "ShaderManager.h"
#include "imgui/imgui.h"
#include <fstream>
#include <Graphics/Graphics.h>

ShaderEditor::ShaderEditor(std::shared_ptr<ShaderManager> shaderManager)
	:
	m_shaderManager(shaderManager),
	m_needsReload(false),
    m_needsSaving(false)
{
	//load the initial code form the shader manager
	m_vertexShaderCode = shaderManager->GetVertexShaderCode();
	m_pixelShaderCode = shaderManager->GetPixelShaderCode();
}


void ShaderEditor::Render(Graphics& gfx) {
    ImGui::Text("Shader Editor");

    static char vertexShaderBuffer[1024 * 16];
    static char pixelShaderBuffer[1024 * 16];

    // Copy the current shader code to the buffers
    strncpy_s(vertexShaderBuffer, m_vertexShaderCode.c_str(), sizeof(vertexShaderBuffer));
    vertexShaderBuffer[sizeof(vertexShaderBuffer) - 1] = '\0'; // Ensure null termination
    strncpy_s(pixelShaderBuffer, m_pixelShaderCode.c_str(), sizeof(pixelShaderBuffer));
    pixelShaderBuffer[sizeof(pixelShaderBuffer) - 1] = '\0'; // Ensure null termination

    ImGui::Text("Vertex Shader:");
    if (ImGui::InputTextMultiline("##vertex", vertexShaderBuffer, sizeof(vertexShaderBuffer), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16))) {
        m_vertexShaderCode = vertexShaderBuffer;
        m_needsReload = true;
        m_needsSaving = true;
    }
    ImGui::Text("Pixel Shader:");
    if (ImGui::InputTextMultiline("##pixel", pixelShaderBuffer, sizeof(pixelShaderBuffer), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16))) {
        m_pixelShaderCode = pixelShaderBuffer;
        m_needsReload = true;
        m_needsSaving = true;
    }
    if (m_needsReload && ImGui::Button("compile")) {
        m_shaderManager->SetVertexShaderCode(m_vertexShaderCode);
        m_shaderManager->SetPixelShaderCode(m_pixelShaderCode);
        m_shaderManager->ReloadShaders(gfx);
        m_needsReload = false;
        m_needsSaving = true;
    }
    if (m_needsSaving && ImGui::Button("save"))
    {
        std::ofstream vertexShaderFile(m_shaderManager->getVertexShaderPath());
        if (vertexShaderFile.is_open()) {
            vertexShaderFile << m_vertexShaderCode;
            vertexShaderFile.close();
        }
        std::ofstream pixelShaderFile(m_shaderManager->getPixelShaderPath());
        if (pixelShaderFile.is_open()) {
            pixelShaderFile << m_pixelShaderCode;
            pixelShaderFile.close();
        }
        m_needsSaving = false;
    }

  
}
