//
// Created by zach on 2023-04-30.
//

#ifndef GATHERSUN_PAUSEMENU_H
#define GATHERSUN_PAUSEMENU_H

#include "Core.h"
#include "Menu.h"

namespace gathersun::ui {

    class PauseMenu : public Menu {
    public:
        explicit PauseMenu(const glm::ivec2 size, entt::registry &registry);

        ~PauseMenu() = default;

        void Resize(const glm::ivec2 size, entt::registry &registry) override;

    private:
        static void resumeButtonPressed_(entt::registry &registry);

        static void optionsButtonPressed_(entt::registry &registry);

        static void loadButtonPressed_(entt::registry &registry);

        static void saveButtonPressed_(entt::registry &registry);

        static void exitButtonPressed_(entt::registry &registry);
    };
} // gathersun::ui

#endif // GATHERSUN_PAUSEMENU_H
