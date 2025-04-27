#include "ClientApp.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <iostream>

ClientApp::ClientApp()
    :
    window(L"engine", L"DirectX", 1366, 768),
    imgui(),
	client(),
    loginWin(window,client),
    projectManager(window,&client),
    loggedIn(false)

{
   
}


int ClientApp::createLoop()
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
void ClientApp::Render()
{
    projectManager.ShowMenuBar(window.GetInstance());
    if (loggedIn) {
        projectManager.ShowProjectWindow();

        // Check if a project is selected before showing chat window
        if (projectManager.GetSelectedProject()) {
            projectManager.LoadSelectedProject();
            projectManager.ShowChatWindow();
        }

        // Process any pending messages from the client
        client.processMessages();

        projectManager.Update(window.GetInstance());
        projectManager.Render(window.GetInstance());
    }
    window.GetInstance().End();


}

ClientApp::~ClientApp()
{
}


void ClientApp::GameProjects()
{
    
}