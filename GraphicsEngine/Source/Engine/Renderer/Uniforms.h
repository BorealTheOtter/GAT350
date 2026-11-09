#pragma once
#include <glm/glm.hpp>

namespace sr {
#define CHECK_UNIFORM_BOUND(T) static_assert((sizeof(T) % 16) == 0)

    enum class UniformSlot : uint32_t
    {
        Object = 0,
        Frame = 1,
        Light = 2,
        Material = 3
    };

    struct ObjectUniform
    {
        glm::mat4 model;
        glm::mat4 normalMatrix;
    };

    CHECK_UNIFORM_BOUND(ObjectUniform);

    struct FrameUniform
    {
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 viewProjection;
        glm::vec3 camPos;
        float time;
    };

    CHECK_UNIFORM_BOUND(FrameUniform);

    struct MatUniform
    {
        glm::vec4 color;

        glm::vec2 tiling;
        glm::vec2 offset;

        float shine;
        glm::vec3 padding;
    };
    CHECK_UNIFORM_BOUND(MatUniform);
}