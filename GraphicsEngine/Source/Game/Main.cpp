#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>


using namespace sr;

struct vertexUniform {
    glm::mat4 mvp;
};

struct Color4 {
    float r, g, b, a;
};
struct Vertex
{
    Vector3 pos;
    Color4 color;
};

vertexUniform uniform;
glm::vec3 cameraPos{ 0,0,5 };

Transform transform;

std::vector<Vertex> vertices =
{
    Vertex{Vector3{-0.5f, -0.5f, 0.0f}, Color4{1.0f,0.0f,0.0f,1.0f}}, // Bottom-Left
    Vertex{Vector3{0.5f, -0.5f, 0.0f}, Color4{0.0f,1.0f,0.0f,1.0f}}, // Bottom-Right
    Vertex{Vector3{0.5f,  0.5f, 0.0f}, Color4{0.0f,0.0f,1.0f,1.0f}}, // Top-Right
    Vertex{Vector3{-0.5f,  0.5f, 0.0f}, Color4{1.0f,1.0f,1.0f,1.0f}}, // Top-Left
};

std::vector<uint32_t> indices = { 0,1,2,2,3,0 };

int main()
{
    SetWorkingDirectory("Assets");

    //INITIALIZE
    Engine::Get().Initialize();

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer());

    auto ib = std::make_shared<IndexBuffer>();
    ib->Create(indices, Engine::Get().GetRenderer());

    auto model = Resources().Get<Model>("Models/cube-3.obj", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<sr::Shader>("Shaders/unlit.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<sr::Shader>("Shaders/unlit.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    float scale = 0.5f;
    Color4 tint = { 1,1,1,1 };

    vertexUniform uniform;

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

        glm::vec3 dir{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) dir.y += 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir.x -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) dir.y -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir.x += 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) dir.z -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) dir.z += 4;

        cameraPos += dir * dt;

        transform.rot.z = time * 180;

        //View Matrix
        auto view = glm::lookAtRH(cameraPos, cameraPos + glm::vec3{ 0,0,-1 }, glm::vec3{ 0,1,0 });

        //projection matrix
        float aspectRatio = (float)Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspectiveRH(glm::radians(45.0f), aspectRatio, 0.01f, 20.0f);

        uniform.mvp = projection * view * transform.GetMatrix();
            
        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
        Engine::Get().GetRenderer().SetFragmentUniform(0, &tint, sizeof(tint));

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

