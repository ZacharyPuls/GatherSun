//
// Created by zach on 2023-04-30.
//

#ifndef GATHERSUN_UI_PAUSEMENU_H
#define GATHERSUN_UI_PAUSEMENU_H

#include "Core.h"
#include "Menu.h"
#include "scene/Scene.h"
#include "event/EventManager.h"

#include <iostream>

namespace gathersun::ui {

    struct PauseMenu {
        static void ResumeButtonPressed() {
            event::EventManager::Instance().TriggerEvent<event::ResumeEvent>({});
        }

        static void OptionsButtonPressed() {

        }

        static void LoadButtonPressed() {
            std::cout << "Load" << std::endl;
        }

        static void SaveButtonPressed() {
            std::cout << "Save" << std::endl;
        }

        static void ExitButtonPressed() {
            event::EventManager::Instance().TriggerEvent<event::ExitEvent>({});
            std::cout << "Exit" << std::endl;
        }
    };

    Menu CreatePauseMenu(scene::Scene* scene, const std::string &name, const std::string &uiElementsPath, glm::ivec2 size);
}

#endif // GATHERSUN_UI_PAUSEMENU_H
