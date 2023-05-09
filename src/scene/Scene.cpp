//
// Created by zach on 2023-05-05.
//

#include "Scene.h"

namespace gathersun::scene {

    void Scene::SetCamera(Camera camera) {
        camera_ = camera;
    }

    Camera &Scene::GetCamera() {
        return camera_;
    }

    void Scene::AddEntity(entt::registry &registry, entity::Entity &entity) {
        auto entityId = registry.create();
        registry.emplace<entity::Entity>(entityId, entity);
        entities_.push_back(entityId);
    }

    void Scene::AddPlayer(entt::registry &registry, entity::Player player) {
        registry.ctx().emplace<entity::Player>(player);
    }

    void Scene::UpdateEntity(entt::registry &registry, entt::entity entityId, entity::Entity &entity) {
        registry.replace<entity::Entity>(entityId, entity);
    }

    // TODO: instead of keeping Sprite in the registry, just store SpriteComponent and reload the parent Sprite on-demand
    //      (possibly an extra std::vector<Sprite> in class, change Sprite::sprite_ to an entt::entity)
    // TODO: decouple Entity from Sprite
    std::vector<entt::entity> Scene::GetEntities() const {
        return entities_;
    }
}