//
// Created by zach on 2021-09-18.
//

#ifndef GATHERSUN_SCENE_CAMERA_H
#define GATHERSUN_SCENE_CAMERA_H

#include "Core.h"

namespace gathersun::scene {

    class Camera {
    public:
        Camera() = default;

        ~Camera() = default;

        glm::vec3 GetPosition() const;

        void SetPosition(glm::vec3 position);

        entt::entity GetTarget() const;

        void SetTarget(entt::entity target);

        glm::mat4 GetViewMatrix(const entt::registry &registry) const;

        glm::mat4 GetProjectionMatrix() const;

        void OrthographicProjection(float left, float right, float bottom, float top, float near, float far);

    private:
        glm::vec3 position_;
        entt::entity target_;
        glm::mat4 projectionMatrix_;
    };
}

#endif // GATHERSUN_SCENE_CAMERA_H
