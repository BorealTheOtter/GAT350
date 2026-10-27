#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Texture.h"
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>


using namespace sr;

enum class LightType : uint32_t {
    Directional,
    Point,
    Spot
};

//UNIFORMS
struct VertexUniform {
    glm::mat4 mvp;
    glm::mat4 model;
    glm::vec3 camPos;
    float padding1;
};
VertexUniform vertexUniform;

struct LightUniform {

    glm::vec3 pos;
    LightType type;
    glm::vec3 dir;
    float innerCutoff;
    glm::vec4 color;
    glm::vec3 ambient;
    float outerCutoff;
};

LightUniform lightUniform{
    .pos {10, 10, 10},
    .type {LightType::Point},
    .dir {-1,1,0},
    .innerCutoff{0.3},
    .color {1.0, 0.85, 0.60, 0.8},
    .ambient {0.2f},
    .outerCutoff{0.6}
};

struct MatUniform
{
    glm::vec4 color;
    glm::vec2 tiling;
    glm::vec2 offset;
    float shine;
};

MatUniform matUniform{
    .color{1,1,1,1},
    .tiling {1,1},
    .offset {0,0},
    .shine {63}
};

struct Color4 {
    float r, g, b, a;
};
struct Vertex
{
    Vector3 pos;
    Color4 color;
};

Transform transform;

int main()
{
    SetWorkingDirectory("Assets");

    //INITIALIZE
    Engine::Get().Initialize();

    auto model = Resources().Get<Model>("Models/Mayu.obj", Engine::Get().GetRenderer());
    auto texture1 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1001.png", Engine::Get().GetRenderer());
    auto texture2 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1002.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<sr::Shader>("shaders/gouraud.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<sr::Shader>("shaders/gouraud.frag", Engine::Get().GetRenderer(), 1, 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    float scale = 0.5f;

    vertexUniform.camPos = { 0,1,5 };

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

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1)) {
            lightUniform.type = LightType::Directional;
            lightUniform.dir = { -1,1,0 };
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) {
            lightUniform.type = LightType::Point;
            lightUniform.pos = glm::vec3(10, 10, 10);
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3)) {
            lightUniform.type = LightType::Spot;
            lightUniform.pos = glm::vec3(0, 10, 0);
            lightUniform.dir = glm::vec3(0, 1, 0);
        }

        vertexUniform.camPos += dir * dt;

        //matUniform.offset.x = time * 0.2;

        transform.rot.x = time * 90;

        //View Matrix
        auto view = glm::lookAtRH(vertexUniform.camPos, vertexUniform.camPos + glm::vec3{ 0,0,-1 }, glm::vec3{ 0,1,0 });

        //projection matrix
        float aspectRatio = (float)Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspectiveRH(glm::radians(45.0f), aspectRatio, 0.01f, 20.0f);

        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;
            
        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(2, &matUniform, sizeof(matUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(0, &matUniform, sizeof(matUniform));

        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture1);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

