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


Light light1{
    .pos {10, 10, 10},
    .type {LightType::Directional},
    .dir {-1,1,0},
    .range{40},
    .color {1.0, 0.85, 0.60, 0.8},
    //.color {1.0, 0, 0, 0.8},
    .innerCutoff{10},
    .outerCutoff{15}
};

//Light light2{
//    .pos {-10, 10, 5},
//    .type {LightType::Point},
//    .dir {1,1,0},
//    .range{40},
//    .color {0.0, 0.0, 1.0, 0.8},
//    .innerCutoff{10},
//    .outerCutoff{15}
//};
//
//Light light3{
//    .pos {-10, -10, 15},
//    .type {LightType::Point},
//    .dir {1,1,0},
//    .range{40},
//    .color {0.0, 1.0, 0.0, 0.8},
//    .innerCutoff{10},
//    .outerCutoff{15}
//};

ObjectUniform objectUniform;
FrameUniform frameUniform;

LightUniform lightUniform;

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
    //lightUniform.lights[1] = light2;
    //lightUniform.lights[2] = light3;
    lightUniform.numLights = 1;
    lightUniform.ambient = glm::vec3(0.5, 0.425, 0.30);

    //INITIALIZE
    Engine::Get().Initialize();

    Scene scene;
    scene.Load("Scenes/scene.json");

    auto model = Resources().Get<Model>("Models/Mayu.obj", Engine::Get().GetRenderer());
    auto texture1 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1001.png", Engine::Get().GetRenderer());
    auto texture2 = Resources().Get<Texture>("Textures/Mayu/TORA_base_color_1002.png", Engine::Get().GetRenderer());

    //auto vshader = Resources().Get<sr::Shader>("shaders/gouraud.vert", Engine::Get().GetRenderer(), 4);
    //auto fshader = Resources().Get<sr::Shader>("shaders/gouraud.frag", Engine::Get().GetRenderer(), 1, 1);

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



        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1)) {
            lightUniform.lights[0].type = LightType::Directional;
            lightUniform.lights[0].dir = { -1,1,0 };
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) {
            lightUniform.lights[0].type = LightType::Point;
            lightUniform.lights[0].pos = glm::vec3(-3, 7, 2);
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3)) {
            lightUniform.lights[0].type = LightType::Spot;
            lightUniform.lights[0].pos = glm::vec3(0, 15, 0);
            lightUniform.lights[0].dir = glm::vec3(0, 1, 0);
            lightUniform.lights[0].innerCutoff = glm::cos(glm::radians(6.0));
            lightUniform.lights[0].outerCutoff = glm::cos(glm::radians(5.0));
        }

        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_I)) lightUniform.lights[0].pos += glm::vec3{0,0.1,0};
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_K)) lightUniform.lights[0].pos -= glm::vec3{ 0,0.1,0 };
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_J)) lightUniform.lights[0].pos -= glm::vec3{0.1,0,0};
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_L)) lightUniform.lights[0].pos += glm::vec3{0.1,0,0 };
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_O)) lightUniform.lights[0].pos += glm::vec3{0,0,0.1};
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_U)) lightUniform.lights[0].pos -= glm::vec3{ 0,0,0.1 };

        //lightUniform.lights[0].pos = glm::eulerAngleY(time) * glm::vec4{ 0,0,10,1 };
        //lightUniform.lights[1].pos = glm::eulerAngleX(time * 0.5) * glm::vec4{ 0,0,10,1 };
        //lightUniform.lights[2].pos = glm::eulerAngleZ(time * 0.3) * glm::vec4{ 0,0,10,1 };

        //matUniform.offset.x = time * 0.2;
        //transform.scale = glm::vec3(4);
        //transform.rot.x = time * 90;

        auto camera = scene.GetActorByName<Actor>("Camera");
        auto cameraComponent = camera->GetComponent<CameraComponent>();

        frameUniform.camPos = camera->GetTransform().pos;

        objectUniform.model = transform.GetMatrix();
        vertexUniform.mvp = cameraComponent->GetProjection() * cameraComponent->GetView()* vertexUniform.model;
            
        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);

        //gouraud
        //Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        //Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        //Engine::Get().GetRenderer().SetVertexUniform(2, &matUniform, sizeof(matUniform));
        //Engine::Get().GetRenderer().SetFragmentUniform(0, &matUniform, sizeof(matUniform));

        //phong
        Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Object, &objectUniform, sizeof(objectUniform));
        Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Material, &matUniform, sizeof(matUniform));

        Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Object, &objectUniform, sizeof(objectUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Material, &matUniform, sizeof(matUniform));


        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture1);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
        }
        //SHUTDOWN
    Engine::Get().Shutdown();

        return 0;


    }

