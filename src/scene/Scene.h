//
// Created by zach on 2023-05-05.
//

#ifndef GATHERSUN_SCENE_SCENE_H
#define GATHERSUN_SCENE_SCENE_H

#include "Core.h"
#include "Camera.h"

namespace gathersun::entity {
    class Object;
}

namespace gathersun::scene {

    class Scene {
    public:
        Scene() = default;

        ~Scene() = default;

        entity::Object &AddObject(const std::string &name);

        void RemoveObject(entt::entity id);

        entity::Object &GetObject(const std::string &name);

        entity::Object &GetObject(entt::entity id);

        template<typename... ComponentTypes>
        auto View() {
            return registry_.view<ComponentTypes...>();
        }

    private:
        entt::registry registry_;
        std::map<std::string, entity::Object> objectMap_;

        friend class entity::Object;
    };
}

#endif // GATHERSUN_SCENE_SCENE_H
