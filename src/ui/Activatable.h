//
// Created by zach on 2023-05-09.
//

#ifndef GATHERSUN_UI_ACTIVATABLE_H
#define GATHERSUN_UI_ACTIVATABLE_H

#include "Core.h"
#include "scene/Scene.h"

namespace gathersun::ui {

    struct Activatable {
        glm::vec4 ActiveTexcoords;
        std::function<void(void)> OnClick;
        bool Active = false;
    };

    struct Selectable {
        glm::vec4 ActiveTexcoords;
        std::function<void(const std::string&)> OnSelect;
        bool Selected = false;
    };
}

#endif // GATHERSUN_UI_ACTIVATABLE_H
