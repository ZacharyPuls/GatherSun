//
// Created by zach on 2023-05-09.
//

#ifndef GATHERSUN_UI_BUTTON_H
#define GATHERSUN_UI_BUTTON_H

#include "Core.h"
#include "entity/Object.h"
#include "ui/Activatable.h"
#include "render/Renderable.h"

namespace gathersun::ui {

    class Button : entity::Object {
    public:
        ~Button() = default;

        glm::vec4 GetTexcoords() {
            auto &activatable = GetComponent<Activatable>();
            auto &renderable = GetComponent<render::Renderable>();
            return activatable.Active ? activatable.ActiveTexcoords : renderable.Texcoords;
        }
    private:

    };
}

#endif // GATHERSUN_UI_BUTTON_H
