#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Texture.h"

#include "Framework/Scene.h"
#include "Components/CameraComponent.h"
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

using namespace sr;

//UNIFORMS

Transform transform;

int main()
{
    SetWorkingDirectory("Assets");

    //INITIALIZE
    Engine::Get().Initialize();

    Scene scene;
    scene.Load("Scenes/scene2.json");
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
        Engine::Get().GetEditor().Update();

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

