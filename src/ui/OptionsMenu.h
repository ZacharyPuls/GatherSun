//
// Created by zach on 2023-05-05.
//

#ifndef GATHERSUN_OPTIONSMENU_H
#define GATHERSUN_OPTIONSMENU_H

#include "event/EventManager.h"
#include "ui/Menu.h"
#include "scene/Scene.h"

#include <iostream>

namespace gathersun::ui {

    struct OptionsMenu {
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

    static Menu CreateOptionsMenu(scene::Scene* scene, const std::string &name, const std::string &uiElementsPath, const glm::ivec2 size) {
        Menu menu(scene, name, uiElementsPath);
        menu.AddButton({}, {320, 128}, OptionsMenu::ResumeButtonPressed, "Resume");
        menu.AddButton({}, {320, 128}, OptionsMenu::OptionsButtonPressed, "Options");
        menu.AddButton({}, {320, 128}, OptionsMenu::LoadButtonPressed, "Load");
        menu.AddButton({}, {320, 128}, OptionsMenu::SaveButtonPressed, "Save");
        menu.AddButton({}, {320, 128}, OptionsMenu::ExitButtonPressed, "Exit");
        menu.Resize(size);
        return menu;
    }
}

#endif // GATHERSUN_OPTIONSMENU_H
