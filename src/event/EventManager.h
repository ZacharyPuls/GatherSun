//
// Created by zach on 2023-05-01.
//

#ifndef GATHERSUN_EVENT_EVENTMANAGER_H
#define GATHERSUN_EVENT_EVENTMANAGER_H

#include <utility>

#include "Core.h"
#include "Event.h"

namespace gathersun::event {

    // TODO: Refactor the event pump to not rely on a singleton EventManager - main issue is the requirement for static input callbacks.
    class EventManager {
    public:
        void operator=(const EventManager &) = delete;

        template<typename EventType, auto Listener>
        void ConnectListener(auto instance) {
            dispatcher_->sink<EventType>().template connect<Listener>(instance);
        }

        template<typename EventType>
        void TriggerEvent(const EventType &event) {
            dispatcher_->trigger(event);
        }

        static EventManager Instance();

    private:
        EventManager();

        EventManager(EventManager &) = default;

        std::shared_ptr<entt::dispatcher> dispatcher_;
        static EventManager instance_;
    };
}

#endif // GATHERSUN_EVENT_EVENTMANAGER_H
