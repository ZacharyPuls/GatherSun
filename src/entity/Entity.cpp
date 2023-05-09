//
// Created by zach on 2021-09-18.
//

#include "Entity.h"

namespace gathersun::entity {

    Sprite &Entity::GetSprite() {
        return sprite_;
    }

    void Entity::SetSprite(const Sprite &sprite) {
        sprite_ = sprite;
    }

    glm::vec2 Entity::GetPosition() const {
        return position_;
    }

    void Entity::SetPosition(const glm::vec2 position) {
        position_ = position;
    }
}
