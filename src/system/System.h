//
// Created by zach on 2023-04-29.
//

#ifndef GATHERSUN_SYSTEM_H
#define GATHERSUN_SYSTEM_H

#include "Core.h"
#include "scene/Scene.h"
#include "game/Window.h"
#include "game/GameState.h"

namespace gathersun::system {

    class System {
    public:
        System(scene::Scene *scene, game::Window *window, game::GameState *gameState) : scene_(scene), window_(window),
                                                                                        gameState_(gameState) {}

        virtual ~System() = default;

        virtual void Run(double dt) = 0;

    protected:
        scene::Scene *scene_;
        game::Window *window_;
        game::GameState *gameState_;
    };
}

#endif // GATHERSUN_SYSTEM_H
