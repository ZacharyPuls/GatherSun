//
// Created by zach on 2023-05-01.
//

#ifndef GATHERSUN_EVENT_EVENT_H
#define GATHERSUN_EVENT_EVENT_H

namespace gathersun::event {

    struct Event {
    };

    struct ExitEvent : Event {

    };

    struct MouseButtonEvent : Event {

    };

    struct MouseMotionEvent : Event {

    };

    struct KeyboardEvent : Event {
        enum class EventType {
            PRESS,
            REPEAT,
            RELEASE
        };

        int Key;
        EventType Type;
    };

    struct WindowResizeEvent : Event {

    };

    struct ResumeEvent : Event {

    };

    struct PauseEvent : Event {

    };

    struct PlayerMovementEvent : Event {
        enum class Direction {
            FORWARD,
            LEFT,
            BACKWARD,
            RIGHT
        };

        enum class EventType {
            START,
            CONTINUE,
            STOP
        };

        Direction Direction;
        EventType Type;
    };

    template<typename T>
    struct EventListener {
        virtual void Receive(const T &) = 0;
    };
}

#endif // GATHERSUN_EVENT_EVENT_H
