//
// Created by zach on 2023-05-07.
//

#include "Player.h"

namespace gathersun::entity {

    void Player::StartMoving(Player::Direction direction, float speed) {
        direction_ = direction;
        moving_ = true;
        speed_ = speed;

        switch (direction) {
            case Direction::FORWARD:
                sprite_.SetCycle("WalkingForward");
                break;
            case Direction::LEFT:
                sprite_.SetCycle("WalkingLeft");
                break;
            case Direction::BACKWARD:
                sprite_.SetCycle("WalkingBackward");
                break;
            case Direction::RIGHT:
                sprite_.SetCycle("WalkingRight");
                break;
        }
    }

    void Player::StopMoving() {
        moving_ = false;
    }

    void Player::Update(double dt) {
        if (moving_) {
            sprite_.AdvanceKeyFrame(dt);

            switch (direction_) {
                case Direction::FORWARD:
                    position_.y += speed_ * dt;
                    break;
                case Direction::LEFT:
                    position_.x -= speed_ * dt;
                    break;
                case Direction::BACKWARD:
                    position_.y -= speed_ * dt;
                    break;
                case Direction::RIGHT:
                    position_.x += speed_ * dt;
                    break;
            }
        }
    }
}
