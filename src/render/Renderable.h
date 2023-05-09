//
// Created by zach on 2023-05-07.
//

#ifndef GATHERSUN_RENDER_RENDERABLE_H
#define GATHERSUN_RENDER_RENDERABLE_H

#include "Core.h"
#include "Texture.h"

namespace gathersun::render {

    struct Renderable {
        render::Texture2D Texture;
        bool Visible = true;
    };
}

#endif // GATHERSUN_RENDER_RENDERABLE_H
