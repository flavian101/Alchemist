#include "App.h"



App::App(HINSTANCE hInstance, int showWnd)
    :
    window(hInstance, showWnd, L"engine", L"DirectX", 1920, 1080),
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

     window.Gfx().controlWindow();
    triangle.Draw(window.Gfx());

   // showImguiDemoWindow();
  
   
   
    window.Gfx().End();

  
}

void App::showImguiDemoWindow()
{

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
}