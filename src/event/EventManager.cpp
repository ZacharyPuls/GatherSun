//
// Created by zach on 2023-05-01.
//

#include "EventManager.h"

namespace gathersun::event {

    EventManager EventManager::instance_;

    EventManager EventManager::Instance() {
        return instance_;
    }

    EventManager::EventManager() {
        dispatcher_ = std::make_shared<entt::dispatcher>();
    }
}
