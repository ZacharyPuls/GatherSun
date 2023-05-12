//
// Created by zach on 2021-09-28.
//

#ifndef GATHERSUN_SYSTEM_RENDERSYSTEM_H
#define GATHERSUN_SYSTEM_RENDERSYSTEM_H

#include "Core.h"
#include "System.h"
#include "game/Window.h"
#include "scene/Scene.h"
#include "event/Event.h"
#include "render/Shader.h"
#include "render/VertexArray.h"
#include "render/VertexBuffer.h"
#include "render/Texture.h"

namespace gathersun::system {

    class RenderSystem : public System {
    public:
        RenderSystem(scene::Scene *scene, game::Window *window, game::GameState *gameState);

        void Run(double dt) override;

    private:
        // TODO: Move these to a pool of VAO/VBOs in EnTT, and configurable shaders (also in EnTT), per rendered object
        render::VertexArray vao_;
        render::VertexBuffer vbo_;
        render::Shader defaultShader_;
        render::Shader textShader_;

        void startFrame_();

        void endFrame_();
    };
}

#endif // GATHERSUN_SYSTEM_RENDERSYSTEM_H
