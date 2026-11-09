#pragma once
#include <glm/glm.hpp>

namespace sr {
#define CHECK_UNIFORM_BOUND(T) static_assert((sizeof(T) % 16) == 0)
#define MAX_LIGHTS 8

    enum class LightType : uint32_t {
        Directional,
        Point,
        Spot
    };

    struct Light {
        glm::vec3 pos;
        LightType type;

        glm::vec3 dir;
        float range;

        glm::vec4 color;

        float innerCutoff;
        float outerCutoff;
        glm::vec2 padding;
    };
    CHECK_UNIFORM_BOUND(Light);

    struct LightUniform {
        Light lights[MAX_LIGHTS];
        glm::vec3 ambient;
        uint32_t numLights;
    };

    CHECK_UNIFORM_BOUND(LightUniform);
}