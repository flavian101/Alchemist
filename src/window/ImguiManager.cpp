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
    style.FramePadding = ImVec2(8, 6); // Decrease frame padding
    style.FrameRounding = 4.0f; // Rounded frame corners
    style.FrameBorderSize = 1.0f; // Increase frame border size for better visibility

    // Other styles
    style.WindowPadding = ImVec2(10, 10); // Decrease window padding
    style.IndentSpacing = 20.0f; // Increase indentation spacing
    style.ScrollbarSize = 15.0f; // Increase scrollbar size
    style.ScrollbarRounding = 12.0f; // Rounded scrollbar corners

    // Enable keyboard and gamepad navigation
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    io.Fonts->AddFontFromFileTTF("Assets/font/constan.ttf", 15.0f);

    // Set very dark night blue/black themed colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f); // Night blue black
    colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.08f, 1.00f); // Darker child background
    colors[ImGuiCol_PopupBg] = ImVec4(0.03f, 0.03f, 0.06f, 1.00f); // Dark popup background
    colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.15f, 0.50f); // Slightly lighter border
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Slightly lighter frame background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Hovered frame background
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.14f, 0.20f, 1.00f); // Active frame background
    colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f); // Dark title background
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Active title background
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.02f, 0.02f, 0.05f, 1.00f); // Collapsed title background
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.04f, 0.04f, 0.08f, 1.00f); // Dark menu bar background
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.06f, 0.53f); // Dark scrollbar background
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.15f, 0.15f, 0.30f, 1.00f); // Dark scrollbar grab
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.20f, 0.20f, 0.35f, 1.00f); // Hovered scrollbar grab
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.40f, 1.00f); // Active scrollbar grab
    colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.95f, 1.00f); // Light check mark
    colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.20f, 0.30f, 1.00f); // Dark slider grab
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.25f, 0.35f, 1.00f); // Active slider grab
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f); // Dark button
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Hovered button
    colors[ImGuiCol_ButtonActive] = ImVec4(0.14f, 0.14f, 0.18f, 1.00f); // Active button
    colors[ImGuiCol_Header] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f); // Dark header
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Hovered header
    colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Active header
    colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Dark separator
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Hovered separator
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.14f, 0.20f, 1.00f); // Active separator
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.15f, 0.15f, 0.20f, 0.30f); // Dark resize grip
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.20f, 0.20f, 0.25f, 0.60f); // Hovered resize grip
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.25f, 0.30f, 0.90f); // Active resize grip
    colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f); // Light plot lines
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.75f, 0.75f, 0.80f, 1.00f); // Hovered plot lines
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.80f, 0.80f, 0.85f, 1.00f); // Light plot histogram
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.90f, 0.95f, 1.00f); // Hovered plot histogram
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.50f, 0.55f, 0.35f); // Light text selected background
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.55f, 0.90f); // Light drag drop target
    colors[ImGuiCol_NavHighlight] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Dark navigation highlight
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f); // Navigation window highlight
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.25f, 0.20f); // Navigation window dim background
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.25f, 0.25);


}

ImguiManager::~ImguiManager()
{
    // Destroy ImGui context
    ImGui::DestroyContext();
}