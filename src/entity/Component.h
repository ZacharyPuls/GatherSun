//
// Created by zach on 2021-09-26.
//

#ifndef GATHERSUN_ENTITY_COMPONENT_H
#define GATHERSUN_ENTITY_COMPONENT_H

#include "Util.h"
#include "Core.h"

namespace gathersun::entity {
    struct TransformComponent {
        glm::vec3 Translation = glm::vec3(0.0f);
        glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);

        glm::mat4 GetModelMatrix() const {
            glm::mat4 transform(1.0f);
            transform = glm::translate(transform, Translation);
            transform *= glm::mat4_cast(Rotation);
            transform = glm::scale(transform, Scale);
            return transform;
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TransformComponent, Translation, Rotation, Scale);
    };

    struct VelocityComponent {
        glm::vec3 Velocity = glm::vec3(0.0f);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(VelocityComponent, Velocity);
    };

    struct RotatingEffectComponent {
        float Speed = 0.0f;
        glm::vec3 Axis = glm::vec3(0.0f);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(RotatingEffectComponent, Speed, Axis);
    };

    struct ColorShiftEffectComponent {
        float Speed = 0.0f;
        glm::vec4 StartColor = glm::vec4(0.0f);
        glm::vec4 EndColor = glm::vec4(0.0f);
        float Progress = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ColorShiftEffectComponent, Speed, StartColor, EndColor, Progress);
    };

    struct SizeComponent {
        glm::vec3 Size = glm::vec3(1.0f);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SizeComponent, Size);
    };
}

#endif // GATHERSUN_ENTITY_COMPONENT_H
