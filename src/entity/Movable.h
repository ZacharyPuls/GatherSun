//
// Created by zach on 2023-05-09.
//

#ifndef GATHERSUN_ENTITY_MOVABLE_H
#define GATHERSUN_ENTITY_MOVABLE_H

#include "Core.h"
#include "render/Animatable.h"
#include "render/Renderable.h"

namespace gathersun::entity {

    struct Movable {
        enum class Direction {
            FORWARD,
            LEFT,
            BACKWARD,
            RIGHT
        };

        Direction Direction;
        float Speed;
        bool Moving;

        void StartMoving(render::Animatable &animatableComponent, enum Direction direction, float speed = 30.0f) {
            Moving = true;
            Speed = speed;

            switch (direction) {
                case Direction::FORWARD:
                    animatableComponent.SetCycle("WalkingForward");
                    break;
                case Direction::LEFT:
                    animatableComponent.SetCycle("WalkingLeft");
                    break;
                case Direction::BACKWARD:
                    animatableComponent.SetCycle("WalkingBackward");
                    break;
                case Direction::RIGHT:
                    animatableComponent.SetCycle("WalkingRight");
                    break;
            }

            Direction = direction;
        }

        void StopMoving() {
            Moving = false;
        }

        void Update(render::Animatable &animatableComponent, render::Position &positionComponent,
                    render::Renderable &renderableComponent, const double dt) {
            if (Moving) {
                animatableComponent.AdvanceKeyFrame(dt);

                switch (Direction) {
                    case Direction::FORWARD:
                        positionComponent.Position.y += Speed * dt;
                        break;
                    case Direction::LEFT:
                        positionComponent.Position.x -= Speed * dt;
                        break;
                    case Direction::BACKWARD:
                        positionComponent.Position.y -= Speed * dt;
                        break;
                    case Direction::RIGHT:
                        positionComponent.Position.x += Speed * dt;
                        break;
                }
            }
            auto texcoords = animatableComponent.KeyFrames[animatableComponent.CurrentKeyFrame].AtlasBounds;
            texcoords.x /= static_cast<float>(renderableComponent.Texture->GetWidth());
            texcoords.y /= static_cast<float>(renderableComponent.Texture->GetHeight());
            texcoords.z /= static_cast<float>(renderableComponent.Texture->GetWidth());
            texcoords.w /= static_cast<float>(renderableComponent.Texture->GetHeight());
            renderableComponent.Texcoords = texcoords;
        }
    };
}

#endif // GATHERSUN_ENTITY_MOVABLE_H
