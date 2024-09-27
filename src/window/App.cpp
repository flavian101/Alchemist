#include "App.h"



App::App()
    :
    window(L"engine", L"DirectX", 1366, 768),
    timer(),
    sceneManager(&window)
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
        sceneManager.Update(window.GetInstance(),timer.Tick());
        Render();
    }

 
}
void App::Render()
{
    window.GetInstance().ClearDepthColor(0.0f, 0.0f, 0.0f);
    sceneManager.Render(window.GetInstance());
    window.GetInstance().End();

}

App::~App()
{
}


void App::GameProjects()
{
    
}