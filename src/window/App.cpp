#include "App.h"



App::App(HINSTANCE hInstance, int showWnd)
    :
    window(hInstance, showWnd, L"engine", L"DirectX", 1270, 720),
    camera(XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
    triangle(window.Gfx())
   
{
    input = std::make_unique<Input>(window);
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
   // const auto dt = timer.Mark() * speedfactor;
    const float dt = timer.Peek();
    window.Gfx().ClearDepthColor(0.0f,0.0f,0.0f);

    window.Gfx().SetViewMatrix(camera.GetView());
    input->DetectInput(dt,camera);
    window.Gfx().controlWindow();

    triangle.Draw(window.Gfx(),camera.GetPos(), camera.GetTarget());

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