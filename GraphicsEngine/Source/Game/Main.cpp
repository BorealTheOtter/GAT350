#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sr;

struct vertexUniform {
    glm::mat4 model;
};

struct Color4 {
    float r, g, b, a;
};
struct Vertex
{
    Vector3 pos;
    Color4 color;
};



std::vector<Vertex> vertices =
{
    Vertex{Vector3{-0.82f, -0.53f, 0.0f}, Color4{1.0f,0.0f,0.0f,1.0f}}, // Bottom-Left
    Vertex{Vector3{0.64f, -0.74f, 0.0f}, Color4{0.0f,1.0f,0.0f,1.0f}}, // Bottom-Right
    Vertex{Vector3{0.26f,  0.83f, 0.0f}, Color4{0.0f,0.0f,1.0f,1.0f}}, // Top-Middle
};

int main()
{
    SetWorkingDirectory("Assets");

    //INITIALIZE
    Engine::Get().Initialize();

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer().GetGPUDevice());

    auto vshader = Resources().Get<sr::Shader>("Shaders/position.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<sr::Shader>("Shaders/color.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();
    pipeline->AddVertexBuffer(sizeof(Vertex));
    //pos
    pipeline->AddVertexAttribute(
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, pos));
    //color
    pipeline->AddVertexAttribute(
        1,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
        offsetof(Vertex, color));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    float scale = 0.5f;
    Color4 color = { 0,0,0,1 };

    vertexUniform uniform;

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

        scale = sin(Engine::Get().GetTime().GetTime());

        uniform.model = glm::mat4(1.0f);

        auto mxs = glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.5f, 0.5f, 0.5f));
        auto mxr = glm::rotate(glm::mat4{ 1.0f }, glm::radians(90.0f) * time, glm::vec3(0.0f, 0.0f, 1.0f));
        auto mxt = glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.5f, 0.0f, 0.0f));
        uniform.model = mxs * mxr * mxt;


        //uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));
        //uniform.model = glm::rotate(uniform.model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
            
        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexBuffer(*vb);

        Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
        

        Engine::Get().GetRenderer().Draw(vb->GetVertexCount());

        Engine::Get().GetRenderer().EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

