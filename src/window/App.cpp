#include "App.h"
#include "Sample\SampleScene.h"



App::App(HINSTANCE hInstance, int showWnd)
    :
    window(hInstance, showWnd, L"engine", L"DirectX", 1270, 720),
    s_scene("Sample", window.Gfx(), window),
    timer()
   // manager(window.Gfx())
{
    //SampleScene* samp = new SampleScene("Sample", window.Gfx());
   // manager.AddScene(samp);
   // manager.SetActiveScene("Sample");
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
        //manager.Update(timer.Peek());
        s_scene.Update(timer.Tick());
        Render();
    }



}
void App::Render()
{
    //timer
   // const auto dt = timer.Mark() * speedfactor;
    const float dt = timer.Tick();
    window.Gfx().ClearDepthColor(0.0f,0.0f,0.0f);

   //manager.Render();
    s_scene.Render();

    window.Gfx().End();

  
}
App::~App()
{
  //  for (auto scene : manager.GetScenes())
  //  {
  //      delete scene;
  //  }
}


void App::showImguiDemoWindow()
{

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
}