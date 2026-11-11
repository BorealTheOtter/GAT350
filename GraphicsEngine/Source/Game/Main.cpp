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
    scene.Load("Scenes/scene.json");

    auto model = Resources().Get<Model>("Models/Mayu.obj", Engine::Get().GetRenderer());
    auto texture1 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1001.png", Engine::Get().GetRenderer());
    auto texture2 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1002.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<sr::Shader>("shaders/phong.vert", Engine::Get().GetRenderer(), 4);
    auto fshader = Resources().Get<sr::Shader>("shaders/phong.frag", Engine::Get().GetRenderer(), 4, 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    float scale = 0.5f;

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

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
        float time = Engine::Get().GetTime().GetTime();

        scene.Update(dt, Engine::Get().GetScreen().x, Engine::Get().GetScreen().y);

        auto& renderer = Engine::Get().GetRenderer();
            
        //RENDER
        renderer.BeginFrame();

        renderer.SetPipeline(*pipeline);
        renderer.SetFragmentTexture(0, *texture1);

        scene.Draw(renderer);

        renderer.EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

