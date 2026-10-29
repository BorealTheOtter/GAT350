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

#define CHECK_UNIFORM_BOUND(T) static_assert((sizeof(T) % 16) == 0);
#define MAX_LIGHTS 8

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

CHECK_UNIFORM_BOUND(VertexUniform)

VertexUniform vertexUniform;

struct Light {
    glm::vec3 pos;
    LightType type;

    glm::vec3 dir;
    float range;

    glm::vec4 color;

    glm::vec3 ambient;
    float innerCutoff;

    float outerCutoff;
    glm::vec3 padding;
};
CHECK_UNIFORM_BOUND(Light)

Light light1{
    .pos {10, 10, 10},
    .type {LightType::Point},
    .dir {-1,1,0},
    .range{40},
    //.color {1.0, 0.85, 0.60, 0.8}
    .color {1.0, 0, 0, 0.8},
    .ambient {0.2f},
    .innerCutoff{10},
    .outerCutoff{15}
};

Light light2{
    .pos {-10, 10, 10},
    .type {LightType::Point},
    .dir {1,1,0},
    .range{40},
    .color {0.0, 0.0, 1.0, 0.8},
    .ambient {0.2f},
    .innerCutoff{10},
    .outerCutoff{15}
};

struct LightUniform {
    Light lights[MAX_LIGHTS];
    glm::vec3 ambient;
    uint32_t numLights;
};

LightUniform lightUniform;




CHECK_UNIFORM_BOUND(LightUniform)

struct MatUniform
{
    glm::vec4 color;

    glm::vec2 tiling;
    glm::vec2 offset;

    float shine;
    glm::vec3 padding;
};
CHECK_UNIFORM_BOUND(MatUniform)

MatUniform matUniform{
    .color{1,1,1,1},
    .tiling {1,1},
    .offset {0,0},
    .shine {64}
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

    lightUniform.lights[0] = light1;
    lightUniform.lights[1] = light2;
    lightUniform.numLights = 2;
    lightUniform.ambient = glm::vec3(0.5, 0.425, 0.30);

    //INITIALIZE
    Engine::Get().Initialize();

    auto model = Resources().Get<Model>("Models/Mayu.obj", Engine::Get().GetRenderer());
    auto texture1 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1001.png", Engine::Get().GetRenderer());
    auto texture2 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1002.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<sr::Shader>("shaders/phong.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<sr::Shader>("shaders/phong.frag", Engine::Get().GetRenderer(), 3, 1);

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
        glm::vec3 rot{ 0 };

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) dir.y += 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir.x -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) dir.y -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir.x += 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) dir.z -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) dir.z += 4;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_I)) rot.y += 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_J)) rot.x -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_K)) rot.y -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_L)) rot.x += 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_O)) rot.z -= 4;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_U)) rot.z += 4;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1)) {
            lightUniform.lights[0].type = LightType::Directional;
            lightUniform.lights[0].dir = { -1,1,0 };
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) {
            lightUniform.lights[0].type = LightType::Point;
            lightUniform.lights[0].pos = glm::vec3(10, 10, 10);
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3)) {
            lightUniform.lights[0].type = LightType::Spot;
            lightUniform.lights[0].pos = glm::vec3(0, 15, 0);
            lightUniform.lights[0].dir = glm::vec3(0, 1, 0);
            lightUniform.lights[0].innerCutoff = glm::cos(glm::radians(15.0));
            lightUniform.lights[0].outerCutoff = glm::cos(glm::radians(5.0));
        }

        vertexUniform.camPos += dir * dt;

        lightUniform.lights[0].pos = glm::eulerAngleY(time) * glm::vec4{ 0,0,10,1 };
        lightUniform.lights[1].pos = glm::eulerAngleX(time * 0.5) * glm::vec4{ 0,0,10,1 };
        

        //matUniform.offset.x = time * 0.2;
        //transform.scale = glm::vec3(4);
        //transform.rot.x = time * 90;

        //View Matrix
        auto view = glm::lookAtRH(vertexUniform.camPos, vertexUniform.camPos + glm::vec3{ 0,0,-1 }, glm::vec3{ 0,1,0 });

        //projection matrix
        float aspectRatio = (float)Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspectiveRH(glm::radians(45.0f), aspectRatio, 0.01f, 200.0f);

        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;
            
        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(2, &matUniform, sizeof(matUniform));

        Engine::Get().GetRenderer().SetFragmentUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(2, &matUniform, sizeof(matUniform));


        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture1);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

