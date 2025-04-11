#include "App.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <iostream>

App::App()
    :
    window(L"engine", L"DirectX", 1366, 768),
    imgui(),
	client(),
    loginWin(window,client),
    projectManager(window),
    loggedIn(false)

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


        window.GetInstance().ClearDepthColor(0.0f, 0.0f, 0.0f);

       
        if (!loggedIn) {
            if (!loginWin.Show()) {
                window.GetInstance().End();
                continue;
            }
            else
            {
                loggedIn = true; 
            }
        }
        Render();

       
    }
 
}
void App::Render()
{
    projectManager.ShowMenuBar(window.GetInstance());
    if (loggedIn) {
        projectManager.ShowProjectWindow();
        projectManager.LoadSelectedProject();
		projectManager.ShowChatWindow(client);
        projectManager.Update(window.GetInstance());
        projectManager.Render(window.GetInstance());
    }
    window.GetInstance().End();


}

App::~App()
{
}


void App::GameProjects()
{
    
}