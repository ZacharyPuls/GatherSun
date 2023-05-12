//
// Created by zach on 2023-05-11.
//

#include "PauseMenu.h"

namespace gathersun::ui {

    Menu CreatePauseMenu(scene::Scene* scene, const std::string &name, const std::string &uiElementsPath, const glm::ivec2 size) {
        Menu menu(scene, name, uiElementsPath);
        menu.AddButton({}, {320, 128}, PauseMenu::ResumeButtonPressed, "Resume");
        menu.AddButton({}, {320, 128}, PauseMenu::OptionsButtonPressed, "Options");
        menu.AddButton({}, {320, 128}, PauseMenu::LoadButtonPressed, "Load");
        menu.AddButton({}, {320, 128}, PauseMenu::SaveButtonPressed, "Save");
        menu.AddButton({}, {320, 128}, PauseMenu::ExitButtonPressed, "Exit");
        menu.Resize(size);
        return menu;
    }
}