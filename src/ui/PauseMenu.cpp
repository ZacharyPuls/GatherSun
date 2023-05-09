//
// Created by zach on 2023-04-30.
//

#include <iostream>
#include "event/Event.h"
#include "event/EventManager.h"
#include "PauseMenu.h"

namespace gathersun::ui {

    PauseMenu::PauseMenu(const glm::ivec2 size, entt::registry &registry) : Menu("pause_menu", "res/menu/ui-elements.json") {
        AddButton(registry, {}, {320, 128}, resumeButtonPressed_, "Resume");
        AddButton(registry, {}, {320, 128}, optionsButtonPressed_, "Options");
        AddButton(registry, {}, {320, 128}, loadButtonPressed_, "Load");
        AddButton(registry, {}, {320, 128}, saveButtonPressed_, "Save");
        AddButton(registry, {}, {320, 128}, exitButtonPressed_, "Exit");
        PauseMenu::Resize(size, registry);
    }

    void PauseMenu::resumeButtonPressed_(entt::registry &registry) {
        event::EventManager::Instance().TriggerEvent<event::ResumeEvent>({});
    }

    void PauseMenu::optionsButtonPressed_(entt::registry &registry) {

    }

    void PauseMenu::loadButtonPressed_(entt::registry &registry) {
        std::cout << "Load" << std::endl;
    }

    void PauseMenu::saveButtonPressed_(entt::registry &registry) {
        std::cout << "Save" << std::endl;
    }

    void PauseMenu::exitButtonPressed_(entt::registry &registry) {
        event::EventManager::Instance().TriggerEvent<event::ExitEvent>({});
        std::cout << "Exit" << std::endl;
    }

    void PauseMenu::Resize(const glm::ivec2 size, entt::registry &registry) {
        const auto &components = GetComponents();
        // Iterate backwards, as the buttons are expected to be FIFO
        auto i = 0;
        std::for_each(components.rbegin(), components.rend(), [&](const auto &component) {
            // TODO: Don't hardcode component type
            auto &button = registry.get<ui::ButtonComponent>(component);
            auto x = static_cast<int>((size.x - button.Size.x) * 0.5f);
            // TODO: Padding
            auto y = static_cast<int>((size.y - (button.Size.y * components.size())) * 0.5f) + (i * button.Size.y);
            // TODO: Element resizing to fit window scale/aspect ratio
            button.Position = {x, y};
            ++i;
        });
    }
}