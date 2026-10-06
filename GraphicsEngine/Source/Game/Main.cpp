#include "Engine.h"
#include "SpaceGame/Assets.h"
#include <memory>

using namespace sr;

int main()
{
    SetWorkingDirectory("Assets");

    //INITIALIZE
    Engine::Get().Initialize();

    //MAIN LOOP
    bool quit = false;
    while (!quit) {

        //UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_ESCAPE)) {
                quit = true;
            }
        }

        Engine::Get().Update();

        float dt = Engine::Get().GetTime().GetDeltaTime();
            
        //RENDER
            Engine::Get().GetRenderer().SetColor(0.0f, 0.0f, 0.0f);
            Engine::Get().GetRenderer().Clear();

            Engine::Get().GetRenderer().SetColor(1.0f, 1.0f, 1.0f);
            
            Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());

            Engine::Get().GetRenderer().Present();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

