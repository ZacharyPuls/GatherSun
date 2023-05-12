//
// Created by zach on 2023-04-22.
//

#ifndef GATHERSUN_GAME_GAME_H
#define GATHERSUN_GAME_GAME_H

#include <deque>
#include <utility>
#include <iostream>
#include <chrono>
#include "system/EventSystem.h"
#include "Core.h"
#include "system/System.h"
#include "GameState.h"
#include "scene/Scene.h"
#include "render/Texture.h"

namespace gathersun::game {

    class Game {
    public:

        Game();

        ~Game();

        void Run() {
            state_ = GameState::RUNNING;
            lastFrameStartTime_ = std::chrono::steady_clock::now();
            while (state_ == GameState::RUNNING) {
                const auto currentFrameStartTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> deltaTime = currentFrameStartTime - lastFrameStartTime_;
                lastFrameStartTime_ = currentFrameStartTime;
                renderSystem_->Run(deltaTime.count());
                eventSystem_->Run(deltaTime.count());
            }
            state_ = GameState::STOPPED;
        }

    private:
        std::shared_ptr<Window> window_;
        GameState state_ = GameState::STOPPED;
        std::chrono::time_point<std::chrono::steady_clock> lastFrameStartTime_;
        std::shared_ptr<system::System> renderSystem_;
        std::shared_ptr<system::EventSystem> eventSystem_;
        std::shared_ptr<scene::Scene> scene_;
        std::shared_ptr<render::Texture2D> uiTexture_;
    };
}

#endif // GATHERSUN_GAME_GAME_H
