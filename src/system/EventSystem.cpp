//
// Created by zach on 2021-09-18.
//

#include "EventSystem.h"
#include "ui/Menu.h"
#include "ui/PauseMenu.h"
#include "ui/Activatable.h"
#include "game/GameState.h"
#include "event/EventManager.h"
#include "entity/Object.h"
#include "entity/Movable.h"
#include "render/Renderable.h"
#include "render/Animatable.h"

namespace gathersun::system {

    EventSystem::EventSystem(scene::Scene *scene, game::Window *window, game::GameState *gameState) : System(scene,
                                                                                                             window,
                                                                                                             gameState) {
        event::EventManager::Instance().ConnectListener<event::ExitEvent, &EventSystem::exitEventListener_>(this);
        event::EventManager::Instance().ConnectListener<event::MouseButtonEvent, &EventSystem::mouseButtonEventListener_>(
                this);
        event::EventManager::Instance().ConnectListener<event::MouseMotionEvent, &EventSystem::mouseMotionEventListener_>(
                this);
        event::EventManager::Instance().ConnectListener<event::KeyboardEvent, &EventSystem::keyboardEventListener_>(
                this);
        event::EventManager::Instance().ConnectListener<event::WindowResizeEvent, &EventSystem::windowResizeEventListener_>(
                this);
        event::EventManager::Instance().ConnectListener<event::ResumeEvent, &EventSystem::resumeEventListener_>(this);
        event::EventManager::Instance().ConnectListener<event::PauseEvent, &EventSystem::pauseEventListener_>(this);
        event::EventManager::Instance().ConnectListener<event::PlayerMovementEvent, &EventSystem::playerMovementEventListener_>(
                this);
    }

    void EventSystem::Run(double dt) {
        auto &player = scene_->GetObject("Player");
        auto &movableComponent = player.GetComponent<entity::Movable>();
        auto &animatableComponent = player.GetComponent<render::Animatable>();
        auto &positionComponent = player.GetComponent<render::Position>();
        auto &renderableComponent = player.GetComponent<render::Renderable>();
        movableComponent.Update(animatableComponent, positionComponent, renderableComponent, dt);
    }

    void EventSystem::exitEventListener_(event::ExitEvent) {
        *gameState_ = game::GameState::STOPPING;
        // TODO: Allow for multiple windows
        window_->Destroy();
    }

    void EventSystem::mouseButtonEventListener_(event::MouseButtonEvent) {
        if (window_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && lastClickedComponent_ == entt::null) {
            const auto mousePosition = window_->GetMousePosition();
            auto activeMenu = getActiveMenu_();
            if (activeMenu != entt::null) {
                auto &menu = scene_->GetObject(activeMenu);
                auto &menuComponent = menu.GetComponent<ui::Menu>();
                for (auto component: menuComponent.GetElements()) {
                    auto &button = scene_->GetObject(component);
                    auto &renderableComponent = button.GetComponent<render::Renderable>();
                    auto &positionComponent = button.GetComponent<render::Position>();
                    auto &activatableComponent = button.GetComponent<ui::Activatable>();
                    if (mousePosition.x >= positionComponent.Position.x &&
                        mousePosition.x <= (positionComponent.Position.x + renderableComponent.Bounds.z) &&
                        mousePosition.y >= positionComponent.Position.y &&
                        mousePosition.y <= (positionComponent.Position.y + renderableComponent.Bounds.w)) {
                        activatableComponent.Active = true;
                        lastClickedComponent_ = component;
                        break;
                    }
                }
            }
        } else if (!window_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && lastClickedComponent_ != entt::null) {
            auto &button = scene_->GetObject(lastClickedComponent_);
            auto &activatableComponent = button.GetComponent<ui::Activatable>();
            activatableComponent.OnClick();
            activatableComponent.Active = false;
            lastClickedComponent_ = entt::null;
        }
    }

    void EventSystem::mouseMotionEventListener_(event::Event) {

    }

    void EventSystem::keyboardEventListener_(event::KeyboardEvent event) {
        if (event.Key == GLFW_KEY_ESCAPE && event.Type == event::KeyboardEvent::EventType::RELEASE) {
            if (getActiveMenu_() != entt::null) {
                event::EventManager::Instance().TriggerEvent<event::ResumeEvent>({});
            } else {
                event::EventManager::Instance().TriggerEvent<event::PauseEvent>({});
            }
        }

        if (event.Type == event::KeyboardEvent::EventType::PRESS) {
            switch (event.Key) {
                case GLFW_KEY_W:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::FORWARD,
                                    .Type = event::PlayerMovementEvent::EventType::START});
                    break;
                case GLFW_KEY_A:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::LEFT,
                                    .Type = event::PlayerMovementEvent::EventType::START});
                    break;
                case GLFW_KEY_S:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::BACKWARD,
                                    .Type = event::PlayerMovementEvent::EventType::START});
                    break;
                case GLFW_KEY_D:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::RIGHT,
                                    .Type = event::PlayerMovementEvent::EventType::START});
                    break;
            }
        } else if (event.Type == event::KeyboardEvent::EventType::REPEAT) {
            switch (event.Key) {
                case GLFW_KEY_W:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::FORWARD,
                                    .Type = event::PlayerMovementEvent::EventType::CONTINUE});
                    break;
                case GLFW_KEY_A:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::LEFT,
                                    .Type = event::PlayerMovementEvent::EventType::CONTINUE});
                    break;
                case GLFW_KEY_S:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::BACKWARD,
                                    .Type = event::PlayerMovementEvent::EventType::CONTINUE});
                    break;
                case GLFW_KEY_D:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::RIGHT,
                                    .Type = event::PlayerMovementEvent::EventType::CONTINUE});
                    break;
            }
        } else if (event.Type == event::KeyboardEvent::EventType::RELEASE) {
            switch (event.Key) {
                case GLFW_KEY_W:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::FORWARD,
                                    .Type = event::PlayerMovementEvent::EventType::STOP});
                    break;
                case GLFW_KEY_A:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::LEFT,
                                    .Type = event::PlayerMovementEvent::EventType::STOP});
                    break;
                case GLFW_KEY_S:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::BACKWARD,
                                    .Type = event::PlayerMovementEvent::EventType::STOP});
                    break;
                case GLFW_KEY_D:
                    event::EventManager::Instance().TriggerEvent<event::PlayerMovementEvent>(
                            {.Direction = event::PlayerMovementEvent::Direction::RIGHT,
                                    .Type = event::PlayerMovementEvent::EventType::STOP});
                    break;
            }
        }
    }

    void EventSystem::windowResizeEventListener_(event::Event) {
        auto windowSize = window_->GetSize();
        auto activeMenu = getActiveMenu_();
        if (activeMenu != entt::null) {
            auto &menu = scene_->GetObject(activeMenu);
            // TODO: add separate resizable component type?
            auto &menuComponent = menu.GetComponent<ui::Menu>();
            menuComponent.Resize(windowSize);
        }
    }

    void EventSystem::resumeEventListener_(event::ResumeEvent) {
/*        auto activeMenu = getActiveMenu_();
        if (activeMenu != entt::null) {
            auto &menu = scene_->GetObject(activeMenu);
            auto &menuComponent = menu.GetComponent<ui::PauseMenu>();
            menuComponent.SetActive(false);
        }*/

        auto &pauseMenu = scene_->GetObject("PauseMenu");
        auto &pauseMenuComponent = pauseMenu.GetComponent<ui::Menu>();
        pauseMenuComponent.SetActive(false);
    }

    void EventSystem::pauseEventListener_(event::PauseEvent) {
        auto windowSize = window_->GetSize();
        auto &pauseMenu = scene_->GetObject("PauseMenu");
        auto &pauseMenuComponent = pauseMenu.GetComponent<ui::Menu>();
        pauseMenuComponent.SetActive(true);
    }

    void EventSystem::playerMovementEventListener_(event::PlayerMovementEvent playerMovementEvent) {
        // TODO: do we want to avoid using ctx(), and just store the entt::entity ID of Player?
        auto &player = scene_->GetObject("Player");
        auto &movableComponent = player.GetComponent<entity::Movable>();

        if (playerMovementEvent.Type == event::PlayerMovementEvent::EventType::START) {
            auto &animatableComponent = player.GetComponent<render::Animatable>();

            switch (playerMovementEvent.Direction) {
                case event::PlayerMovementEvent::Direction::FORWARD:
                    movableComponent.StartMoving(animatableComponent, entity::Movable::Direction::FORWARD);
                    break;
                case event::PlayerMovementEvent::Direction::LEFT:
                    movableComponent.StartMoving(animatableComponent, entity::Movable::Direction::LEFT);
                    break;
                case event::PlayerMovementEvent::Direction::BACKWARD:
                    movableComponent.StartMoving(animatableComponent, entity::Movable::Direction::BACKWARD);
                    break;
                case event::PlayerMovementEvent::Direction::RIGHT:
                    movableComponent.StartMoving(animatableComponent, entity::Movable::Direction::RIGHT);
                    break;
            }
        } else if (playerMovementEvent.Type == event::PlayerMovementEvent::EventType::CONTINUE) {
            // TODO: do we need this event?
        } else if (playerMovementEvent.Type == event::PlayerMovementEvent::EventType::STOP) {
            movableComponent.StopMoving();
        }
    }

    // TODO: Handle multiple simultaneously active menus
    entt::entity EventSystem::getActiveMenu_() {
        auto menuView = scene_->View<ui::Menu>();
        for (auto &menuEntity: menuView) {
            auto &menu = menuView.get<ui::Menu>(menuEntity);
            if (menu.IsActive()) {
                return menuEntity;
            }
        }
        return entt::null;
    }
}