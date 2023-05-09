//
// Created by zach on 2023-05-05.
//

#ifndef GATHERSUN_SCENE_SCENE_H
#define GATHERSUN_SCENE_SCENE_H

#include "Core.h"
#include "Camera.h"
#include "entity/Entity.h"
#include "entity/Player.h"

namespace gathersun::scene {

    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        void SetCamera(Camera camera);

        Camera &GetCamera();

        void AddEntity(entt::registry &registry, entity::Entity &entity);

        // TODO: do we really want to store player here?
        void AddPlayer(entt::registry &registry, entity::Player player);

        void UpdateEntity(entt::registry &registry, entt::entity entityId, entity::Entity &entity);

        std::vector<entt::entity> GetEntities() const;
    private:
        Camera camera_{};
        std::vector<entt::entity> entities_;
    };
}

#endif // GATHERSUN_SCENE_SCENE_H
