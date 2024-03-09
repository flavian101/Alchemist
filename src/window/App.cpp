#include "App.h"



App::App(HINSTANCE hInstance, int showWnd)
    :
    window(hInstance, showWnd, L"engine", L"DirectX", 1270, 720),
    camera(XMVectorSet(0.0f, 5.0f, -8.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
    triangle(window.Gfx())
{
    window.Gfx().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
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
    float rotationSpeed = 0.02f; 
    float angle = rotationSpeed * dt;
    window.Gfx().ClearDepthColor(0.0f,0.0f,0.0f);

    window.Gfx().SetViewMatrix(camera.GetView());
    window.Gfx().controlWindow();

    triangle.Draw(window.Gfx(),camera.GetPos(), camera.GetTarget(),angle );

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