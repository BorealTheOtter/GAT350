#include "Engine.h"
using namespace sr;

//UNIFORMS

Transform transform;

int main()
{
    SetWorkingDirectory("Assets");

    //INITIALIZE
    Engine::Get().Initialize();

    Scene scene;
    //scene.Load("Scenes/scene-skybox.json");
    scene.Load("Scenes/scene.json");
    auto& renderer = Engine::Get().GetRenderer();

   

    //MAIN LOOP
    bool quit = false;
    while (!quit) {

        //UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Engine::Get().GetEditor().ProcessEvent(event);
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_ESCAPE)) {
                quit = true;
            }
        }

        //ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        //SCENE
        scene.Update(dt, Engine::Get().GetScreen().x, Engine::Get().GetScreen().y);
            
        //EDITOR
        Engine::Get().GetEditor().BeginFrame();
        Engine::Get().GetEditor().Update(scene);

        //RENDER
        renderer.BeginFrame();
        scene.Draw(renderer);

        renderer.EndRenderPass();
        Engine::Get().GetEditor().Draw(renderer);

        renderer.EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

