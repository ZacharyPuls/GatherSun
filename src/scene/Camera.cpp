//
// Created by zach on 2021-09-18.
//

#include "Camera.h"
#include "entity/Entity.h"

namespace gathersun::scene {

    glm::vec3 Camera::GetPosition() const {
        return position_;
    }

    void Camera::SetPosition(glm::vec3 position) {

    }

    entt::entity Camera::GetTarget() const {
        return target_;
    }

    void Camera::SetTarget(entt::entity target) {

    }

    glm::mat4 Camera::GetViewMatrix(const entt::registry &registry) const {
//        auto target = registry.get<entity::Entity>(target_);
        return glm::translate(glm::mat4(1.0f), -position_);
    }

    glm::mat4 Camera::GetProjectionMatrix() const {
        return projectionMatrix_;
    }

    void Camera::OrthographicProjection(float left, float right, float bottom, float top, float near, float far) {
        projectionMatrix_ = glm::ortho(left, right, bottom, top, near, far);
    }
}
