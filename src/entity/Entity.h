//
// Created by zach on 2021-09-18.
//

#ifndef GATHERSUN_ENTITY_ENTITY_H
#define GATHERSUN_ENTITY_ENTITY_H

#include "Core.h"
#include "Sprite.h"

namespace gathersun::entity {

    // TODO: Add EntitySystem that operates on all active Entity objects
    class Entity {
    public:
        Entity() = default;
        Entity(Sprite &sprite, glm::vec2 position) : sprite_(sprite), position_(position) {}

        ~Entity() = default;

        Sprite &GetSprite();

        void SetSprite(const Sprite &sprite);

        glm::vec2 GetPosition() const;

        void SetPosition(glm::vec2 position);

    protected:
        Sprite sprite_;
        glm::vec2 position_;
    };
}

#endif // GATHERSUN_ENTITY_ENTITY_H
