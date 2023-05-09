//
// Created by zach on 2021-09-28.
//

#ifndef GATHERSUN_SYSTEM_RENDERSYSTEM_H
#define GATHERSUN_SYSTEM_RENDERSYSTEM_H

#include "Core.h"
#include "ui/Font.h"
#include "System.h"
#include "game/Window.h"
#include "scene/Scene.h"
#include "event/Event.h"
#include "render/Shader.h"

namespace gathersun::system {

    class RenderSystem : public System {
    public:
        explicit RenderSystem(entt::registry &registry);

        void Run(double dt) override;

    private:
        // TODO: Move these to a pool of VAO/VBOs in EnTT, and configurable shaders (also in EnTT), per rendered object
        render::VertexArray vao_;
        render::VertexBuffer vbo_;
        render::Shader defaultShader_;
        render::Shader textShader_;
        render::Texture2D uiTexture_;
        scene::Scene scene_;

        void startFrame_(const std::shared_ptr<game::Window> &window);

        void endFrame_(const std::shared_ptr<game::Window> &window);
    };
}

#endif // GATHERSUN_SYSTEM_RENDERSYSTEM_H
