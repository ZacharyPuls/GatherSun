//
// Created by zach on 2021-09-18.
//

#include "EventSystem.h"
#include "ui/Menu.h"
#include "ui/PauseMenu.h"
#include "game/GameState.h"
#include "event/EventManager.h"
#include "entity/Entity.h"
#include "entity/Player.h"

namespace gathersun::system {

    EventSystem::EventSystem(entt::registry &registry) : System(registry) {
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
        auto &player = registry_.ctx().get<entity::Player>();
        player.Update(dt);
    }

    void EventSystem::exitEventListener_(event::ExitEvent) {
        auto &state = registry_.ctx().get<game::GameState &>();
        state = game::GameState::STOPPING;
        // TODO: Allow for multiple windows, currently just destroying all windows
        auto view = registry_.view<std::shared_ptr<game::Window>>();
        registry_.destroy(view.begin(), view.end());
    }

    void EventSystem::mouseButtonEventListener_(event::MouseButtonEvent) {
        auto window = registry_.ctx().get<std::shared_ptr<game::Window>>();

        if (window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && lastClickedComponent_ == entt::null) {
            const auto mousePosition = window->GetMousePosition();
            auto activeMenu = getActiveMenu_(registry_);
            if (activeMenu != entt::null) {
                auto menu = registry_.get<std::shared_ptr<ui::Menu>>(activeMenu);
                for (auto component: menu->GetComponents()) {
                    auto &buttonComponent = registry_.get<ui::ButtonComponent>(component);
                    // TODO: don't hard-code the size
                    if (mousePosition.x >= buttonComponent.Position.x &&
                        mousePosition.x <= (buttonComponent.Position.x + 320) &&
                        mousePosition.y >= buttonComponent.Position.y &&
                        mousePosition.y <= (buttonComponent.Position.y + 128)) {
                        buttonComponent.Active = true;
                        lastClickedComponent_ = component;
                        break;
                    }
                }
            }
        } else if (!window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && lastClickedComponent_ != entt::null) {
            auto &buttonComponent = registry_.get<ui::ButtonComponent>(lastClickedComponent_);
            buttonComponent.OnClick(registry_);
            buttonComponent.Active = false;
            lastClickedComponent_ = entt::null;
        }
    }

    void EventSystem::mouseMotionEventListener_(event::Event) {

    }

    void EventSystem::keyboardEventListener_(event::KeyboardEvent event) {
        if (event.Key == GLFW_KEY_ESCAPE && event.Type == event::KeyboardEvent::EventType::RELEASE) {
            if (getActiveMenu_(registry_) != entt::null) {
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
        auto window = registry_.ctx().get<std::shared_ptr<game::Window>>();
        auto windowSize = window->GetSize();
        auto activeMenu = getActiveMenu_(registry_);
        if (activeMenu != entt::null) {
            auto menu = registry_.get<std::shared_ptr<ui::Menu>>(activeMenu);
            menu->Resize(windowSize, registry_);
        }
    }

    void EventSystem::resumeEventListener_(event::ResumeEvent) {
        auto activeMenu = getActiveMenu_(registry_);
        if (activeMenu != entt::null) {
            auto menu = registry_.get<std::shared_ptr<ui::Menu>>(activeMenu);
            menu->SetActive(false);
            registry_.destroy(activeMenu);
        }
    }

    void EventSystem::pauseEventListener_(event::PauseEvent) {
        auto window = registry_.ctx().get<std::shared_ptr<game::Window>>();
        auto windowSize = window->GetSize();
        auto pauseMenu = registry_.create();
        registry_.emplace<std::shared_ptr<ui::Menu>>(pauseMenu, std::make_shared<ui::PauseMenu>(window->GetSize(),
                                                                                                registry_))->SetActive(
                true);
    }

    void EventSystem::playerMovementEventListener_(event::PlayerMovementEvent playerMovementEvent) {
        // TODO: do we want to avoid using ctx(), and just store the entt::entity ID of Player?
        auto &player = registry_.ctx().get<entity::Player>();

        if (playerMovementEvent.Type == event::PlayerMovementEvent::EventType::START) {
            switch (playerMovementEvent.Direction) {
                case event::PlayerMovementEvent::Direction::FORWARD:
                    player.StartMoving(entity::Player::Direction::FORWARD);
                    break;
                case event::PlayerMovementEvent::Direction::LEFT:
                    player.StartMoving(entity::Player::Direction::LEFT);
                    break;
                case event::PlayerMovementEvent::Direction::BACKWARD:
                    player.StartMoving(entity::Player::Direction::BACKWARD);
                    break;
                case event::PlayerMovementEvent::Direction::RIGHT:
                    player.StartMoving(entity::Player::Direction::RIGHT);
                    break;
            }
        } else if (playerMovementEvent.Type == event::PlayerMovementEvent::EventType::CONTINUE) {
            // TODO: do we need this event?
        } else if (playerMovementEvent.Type == event::PlayerMovementEvent::EventType::STOP) {
            player.StopMoving();
        }
    }

    // TODO: Handle multiple simultaneously active menus
    entt::entity EventSystem::getActiveMenu_(entt::registry &registry) {
        auto menuView = registry.view<std::shared_ptr<ui::Menu>>();
        for (auto &menuEntity: menuView) {
            auto menu = menuView.get<std::shared_ptr<ui::Menu>>(menuEntity);
            if (menu->IsActive()) {
                return menuEntity;
            }
        }
        return entt::null;
    }
}