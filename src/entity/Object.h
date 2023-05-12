//
// Created by zach on 2021-09-18.
//

#ifndef GATHERSUN_ENTITY_OBJECT_H
#define GATHERSUN_ENTITY_OBJECT_H

#include "Core.h"
#include "scene/Scene.h"

namespace gathersun::entity {

    class Object {
    public:
        Object() = default;

        Object(entt::entity entityId, scene::Scene *scene) : entityId_(entityId), scene_(scene) {}

        ~Object() = default;

        template<typename ComponentType, typename... Args>
        ComponentType &AddComponent(Args &&... args) {
            return scene_->registry_.emplace<ComponentType>(entityId_, std::forward<Args>(args)...);
        }

        template<typename ComponentType>
        ComponentType &GetComponent() {
            return scene_->registry_.get<ComponentType>(entityId_);
        }

        template<typename ComponentType>
        bool HasComponent() {
            return scene_->registry_.all_of<ComponentType>(entityId_);
        }

        explicit operator entt::entity() const { return entityId_; }

    protected:
        entt::entity entityId_ = entt::null;
        scene::Scene *scene_ = nullptr;
    };
}

#endif // GATHERSUN_ENTITY_OBJECT_H
