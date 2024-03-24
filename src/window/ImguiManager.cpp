#include "ImguiManager.h"

ImguiManager::ImguiManager()
{
    // Check ImGui version and create context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Set custom styles
    ImGuiStyle& style = ImGui::GetStyle();

    // Window styles
    style.WindowRounding = 5.0f; // Rounded window corners
    style.WindowBorderSize = 1.0f; // Window border size
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // Center window titles

    // Frame styles
    style.FramePadding = ImVec2(8, 4); // Decrease frame padding
    style.FrameRounding = 4.0f; // Rounded frame corners
    style.FrameBorderSize = 1.0f; // Increase frame border size for better visibility

    // Colors
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Darker title background
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Slightly brighter active title background
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f); // Darker collapsed title background
    style.Colors[ImGuiCol_Border] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // Slightly darker border
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f); // Slightly darker frame background
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Slightly brighter frame background on hover

    // Other styles
    style.WindowPadding = ImVec2(10, 10); // Decrease window padding
    style.IndentSpacing = 20.0f; // Increase indentation spacing

    // Enable keyboard and gamepad navigation
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

ImguiManager::~ImguiManager()
{
    // Destroy ImGui context
    ImGui::DestroyContext();
}