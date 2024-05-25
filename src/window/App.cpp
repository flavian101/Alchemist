#include "App.h"



App::App(HINSTANCE hInstance, int showWnd)
    :
    window(hInstance, showWnd, L"engine", L"DirectX", 1270, 720),
    sceneManager(window.Gfx(),window),
    timer()
{}


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
        sceneManager.Update(timer.Tick());
        Render();
    }
}
void App::Render()
{
    window.Gfx().ClearDepthColor(0.0f,0.0f,0.0f);
    sceneManager.Render();
    window.Gfx().End();

  
}
App::~App()
{
  // for (auto scene : manager.GetScenes())
  // {
  //     delete scene;
  // }
}


void App::GameProjects()
{
    
}