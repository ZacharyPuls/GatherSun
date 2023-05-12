//
// Created by zach on 2023-05-05.
//

#include "Scene.h"
#include "entity/Object.h"

namespace gathersun::scene {

    entity::Object &Scene::AddObject(const std::string &name) {
        auto entityId = registry_.create();
        auto object = entity::Object{entityId, this};
        objectMap_[name] = object;
        return objectMap_[name];
    }

    void Scene::RemoveObject(entt::entity id) {
        registry_.destroy(id);
        std::erase_if(objectMap_,[id](auto object) {
            return static_cast<entt::entity>(object.second) == id;
        });
    }

    entity::Object &Scene::GetObject(const std::string &name) {
        return objectMap_[name];
    }

    entity::Object &Scene::GetObject(const entt::entity id) {
        return std::find_if(objectMap_.begin(), objectMap_.end(), [id](auto &entry) {
            return static_cast<entt::entity>(entry.second) == id;
        })->second;
    }
}