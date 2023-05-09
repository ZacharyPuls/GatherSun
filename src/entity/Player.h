//
// Created by zach on 2023-05-07.
//

#ifndef GATHERSUN_ENTITY_PLAYER_H
#define GATHERSUN_ENTITY_PLAYER_H

#include "entity/Entity.h"

namespace gathersun::entity {

    class Player : Entity {
    public:
        Player(Sprite &sprite, glm::vec2 position) : Entity(sprite, position) {}
        ~Player() = default;

        enum class Direction {
            FORWARD,
            LEFT,
            BACKWARD,
            RIGHT
        };

        void StartMoving(Direction direction, float speed = 30.0f);

        void StopMoving();

        void Update(double dt);

        glm::vec2 GetPosition() const {return position_;}

        Sprite &GetSprite() {return sprite_;}
    private:
        Direction direction_;
        bool moving_ = false;
        float speed_;
        float subkeyFrame_ = 0.0f;
    };
}

#endif // GATHERSUN_ENTITY_PLAYER_H
