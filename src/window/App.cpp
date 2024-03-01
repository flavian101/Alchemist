#include "App.h"



App::App(HINSTANCE hInstance, int showWnd)
    :
    window(hInstance, showWnd, L"engine", L"DirectX", 1270, 720),
    triangle(window.Gfx())
{

}

int App::createLoop()
{
    while (true)
    {
        //process all messages pending 
        if (const auto ecode = Window::ProcessMessages())
        {
            //if return optional has value, means we'are exiting the program by returning the exit code
            return *ecode;
        }
        Render();
    }



}
void App::Render()
{
    //timer
    const auto dt = timer.Mark() * speedfactor;

    window.Gfx().ClearDepthColor(0.0f,0.0f,0.0f);

    triangle.Draw(window.Gfx());

   // showImguiDemoWindow();
    if (ImGui::Begin("Simulation Speed"))
    {
        //ImGui::SliderFloat("Speed Factor", &speedfactor, 0.0f, 6.0f, "%.4f", 3.2f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();

   
    window.Gfx().End();

  
}

void App::showImguiDemoWindow()
{

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
}