//
// Created by zach on 2021-09-18.
//

#ifndef GATHERSUN_SYSTEM_EVENTSYSTEM_H
#define GATHERSUN_SYSTEM_EVENTSYSTEM_H

#include "game/Window.h"
#include "System.h"
#include "event/Event.h"
#include "game/GameState.h"
#include "ui/Menu.h"

namespace gathersun::system {

    // TODO: log spurious/incorrect events that cannot be handled.
    class EventSystem : public System {
    public:
        EventSystem(scene::Scene *scene, game::Window *window, game::GameState *gameState);

        void Run(double dt) override;

    private:
        entt::entity lastClickedComponent_ = entt::null;

        void exitEventListener_(event::ExitEvent);

        void mouseButtonEventListener_(event::MouseButtonEvent);

        void mouseMotionEventListener_(event::Event);

        void keyboardEventListener_(event::KeyboardEvent);

        void windowResizeEventListener_(event::Event);

        void resumeEventListener_(event::ResumeEvent);

        void pauseEventListener_(event::PauseEvent);

        void playerMovementEventListener_(event::PlayerMovementEvent);

        entt::entity getActiveMenu_();
    };
}

#endif // GATHERSUN_SYSTEM_EVENTSYSTEM_H
