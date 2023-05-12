//
// Created by zach on 2021-09-28.
//

#include "RenderSystem.h"
#include "entity/Component.h"
#include "ui/Menu.h"
#include "ui/Button.h"
#include "ui/Text.h"
#include "render/Shader.h"
#include "render/Renderable.h"

#include <iostream>

namespace gathersun::system {

    RenderSystem::RenderSystem(scene::Scene *scene, game::Window *window, game::GameState *gameState) : System(scene,
                                                                                                               window,
                                                                                                               gameState),
                                                                                                        defaultShader_(
                                                                                                                "res/shader/default.glsl"),
                                                                                                        textShader_(
                                                                                                                "res/shader/text.glsl"),
                                                                                                        vbo_(GL_ARRAY_BUFFER,
                                                                                                             GL_DYNAMIC_DRAW) {
        glEnable(GL_MULTISAMPLE);
        auto camera = scene_->AddObject("Camera");
        camera.AddComponent<scene::Camera>();

//        auto playerSprite = Sprite{"res/sprite/test-spritesheet.png", "res/sprite/test-spritesheet.json"};
//        auto playerEntity = entity::Player(playerSprite, glm::vec2(100.0f, 100.0f));
//        scene_.AddPlayer(registry_, playerEntity);
    }

    void RenderSystem::Run(double dt) {
        scene_->View<ui::RenderableTextComponent, entity::TransformComponent, entity::RotatingEffectComponent>().each(
                [dt](auto &text, auto &transform, auto &rotatingEffect) {
                    transform.Rotation = glm::rotate(transform.Rotation, rotatingEffect.Speed * static_cast<float>(dt),
                                                     rotatingEffect.Axis);
                });

        scene_->View<ui::RenderableTextComponent, entity::ColorShiftEffectComponent>().each(
                [dt](auto &text, auto &colorShiftEffect) {
                    text.Color = glm::mix(colorShiftEffect.StartColor, colorShiftEffect.EndColor,
                                          colorShiftEffect.Progress);
                    colorShiftEffect.Progress += static_cast<float>(colorShiftEffect.Speed * dt);
                    if (colorShiftEffect.Progress > 1.0f) {
                        colorShiftEffect.Progress = 0.0f;
                        const auto temp = colorShiftEffect.StartColor;
                        colorShiftEffect.StartColor = colorShiftEffect.EndColor;
                        colorShiftEffect.EndColor = temp;
                    }
                });

        startFrame_();

        vao_.Bind();
        vbo_.Bind();
        // TODO: actually size this properly
        vbo_.SetData(sizeof(float) * 6 * 4 * 256, nullptr);

        auto width = window_->GetSize().x;
        auto height = window_->GetSize().y;

        auto camera = scene_->GetObject("Camera").GetComponent<scene::Camera>();
        camera.OrthographicProjection(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
        const auto projection = camera.GetProjectionMatrix();

        auto menuView = scene_->View<ui::Menu>();
        // TODO: do we really want to do one draw call per type (menu element, text, etc)?

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // TODO: separate out these draw passes into objects that can be applied/enabled/disabled to a scene at will

        // Game draw pass
        {
            defaultShader_.Bind();
            auto triangleCount = 0;
            /*auto renderableObjects = scene_->View<render::Position, render::Renderable>();
            for (auto object: renderableObjects) {
                auto position = renderableObjects.get<render::Position>(object);
                auto renderable = renderableObjects.get<render::Renderable>(object);
                auto texture = renderable.Texture;
                const auto transform = entity::TransformComponent{glm::vec3(position.Position, 0.0f), {},
                                                                  glm::vec3(1.0f)};
                defaultShader_.UniformMat4(0, projection * transform.GetModelMatrix());
                texture->Bind();
                defaultShader_.UniformTexture(1, 0);

                float left = renderable.Bounds.x;
                float bottom = renderable.Bounds.y;
                float right = renderable.Bounds.z;
                float top = renderable.Bounds.w;

                float textureLeft = renderable.Texcoords.x;
                float textureBottom = renderable.Texcoords.y;
                float textureRight = renderable.Texcoords.z;
                float textureTop = renderable.Texcoords.w;

                float vertices[6][4] = {{left,  top,    textureLeft,  textureTop},
                                        {left,  bottom, textureLeft,  textureBottom},
                                        {right, bottom, textureRight, textureBottom},
                                        {left,  top,    textureLeft,  textureTop},
                                        {right, bottom, textureRight, textureBottom},
                                        {right, top,    textureRight, textureTop}};

                vbo_.SetSubData(sizeof(float) * 4 * 3 * triangleCount, sizeof(vertices), vertices);
                triangleCount += 2;
            }*/

            auto &playerPosition = scene_->GetObject("Player").GetComponent<render::Position>();
            auto &playerRenderable = scene_->GetObject("Player").GetComponent<render::Renderable>();
            const auto transform = entity::TransformComponent{glm::vec3(playerPosition.Position, 0.0f), {},
                                                              glm::vec3(1.0f)};
            defaultShader_.UniformMat4(0, projection * transform.GetModelMatrix());
            playerRenderable.Texture->Bind();
            defaultShader_.UniformTexture(1, 0);

            float left = playerRenderable.Bounds.x;
            float bottom = playerRenderable.Bounds.y;
            float right = playerRenderable.Bounds.z;
            float top = playerRenderable.Bounds.w;

            float textureLeft = playerRenderable.Texcoords.x;
            float textureBottom = playerRenderable.Texcoords.y;
            float textureRight = playerRenderable.Texcoords.z;
            float textureTop = playerRenderable.Texcoords.w;

            float vertices[6][4] = {{left,  top,    textureLeft,  textureTop},
                                    {left,  bottom, textureLeft,  textureBottom},
                                    {right, bottom, textureRight, textureBottom},
                                    {left,  top,    textureLeft,  textureTop},
                                    {right, bottom, textureRight, textureBottom},
                                    {right, top,    textureRight, textureTop}};

            vbo_.SetSubData(sizeof(float) * 4 * 3 * triangleCount, sizeof(vertices), vertices);
            triangleCount += 2;
            vbo_.VertexAttribPointer(0, 2, GL_FLOAT, sizeof(float) * 4, 0);
            vbo_.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(float) * 4, sizeof(float) * 2);
            vao_.Draw(GL_TRIANGLES, 0, triangleCount * 3);
        }

        // UI draw pass
        defaultShader_.Bind();
        for (auto &menuEntity: menuView) {
            auto &menu = menuView.get<ui::Menu>(menuEntity);
            if (menu.IsActive()) {
                for (auto component: menu.GetElements()) {
                    auto triangleCount = 0;
                    auto &element = scene_->GetObject(component);
                    auto &position = element.GetComponent<render::Position>();
                    auto &renderable = element.GetComponent<render::Renderable>();
                    const auto transform = entity::TransformComponent{glm::vec3(position.Position, 0.0f), {},
                                                                      glm::vec3(1.0f)};

                    defaultShader_.UniformMat4(0, projection * transform.GetModelMatrix());
                    renderable.Texture->Bind();
                    defaultShader_.UniformTexture(1, 0);

                    auto texcoords = renderable.Texcoords;

                    if (element.HasComponent<ui::Activatable>()) {
                        auto &activatable = element.GetComponent<ui::Activatable>();
                        if (activatable.Active) {
                            texcoords = activatable.ActiveTexcoords;
                        }
                    }

                    float left = renderable.Bounds.x;
                    float bottom = renderable.Bounds.y;
                    float right = renderable.Bounds.z;
                    float top = renderable.Bounds.w;

                    const auto w = renderable.Texture->GetWidth();
                    const auto h = renderable.Texture->GetHeight();

                    float textureLeft = texcoords.x / static_cast<float>(w);
                    float textureBottom = texcoords.y / static_cast<float>(h);
                    float textureRight = texcoords.z / static_cast<float>(w);
                    float textureTop = texcoords.w / static_cast<float>(h);

                    float vertices[6][4] = {{left,  top,    textureLeft,  textureTop},
                                            {left,  bottom, textureLeft,  textureBottom},
                                            {right, bottom, textureRight, textureBottom},
                                            {left,  top,    textureLeft,  textureTop},
                                            {right, bottom, textureRight, textureBottom},
                                            {right, top,    textureRight, textureTop}};

                    vbo_.SetSubData(sizeof(float) * 4 * 3 * triangleCount, sizeof(vertices), vertices);
                    triangleCount += 2;
                    vbo_.VertexAttribPointer(0, 2, GL_FLOAT, sizeof(float) * 4, 0);
                    vbo_.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(float) * 4, sizeof(float) * 2);
                    vao_.Draw(GL_TRIANGLES, 0, triangleCount * 3);
                }
            }
        }

        // Text draw pass
        textShader_.Bind();
        vbo_.SetData(sizeof(float) * 6 * 4 * 256, nullptr);
        for (auto &menuEntity: menuView) {
            auto &menu = menuView.get<ui::Menu>(menuEntity);
            if (menu.IsActive()) {
                for (auto component: menu.GetElements()) {
                    auto element = scene_->GetObject(component);
                    if (!element.HasComponent<ui::RenderableTextComponent>()) {
                        continue;
                    }
                    auto &positionComponent = element.GetComponent<render::Position>();
                    auto &renderableComponent = element.GetComponent<render::Renderable>();
                    auto &renderableTextComponent = element.GetComponent<ui::RenderableTextComponent>();
                    const auto font = renderableTextComponent.Font;
                    const auto fontTexture = font->GetTexture();
                    const auto transform = entity::TransformComponent{glm::vec3(positionComponent.Position, 0.0f), {},
                                                                      glm::vec3(1.0f)};

                    textShader_.UniformMat4(0, projection * transform.GetModelMatrix());
                    fontTexture->Bind();
                    textShader_.UniformTexture(1, 0);
                    textShader_.UniformVec3(2, renderableTextComponent.Color);
                    const auto &atlas = font->GetAtlas();

                    // TODO: is this copy really worth it to avoid std::map::at()?
                    auto glyphs = font->GetGlyphs();

                    float x = 0.0f;
                    float y = 0.0f;
                    // float scale = atlas.size;
                    float scale = renderableTextComponent.Scale;

                    float buttonWidth = renderableComponent.Bounds.z;
                    float buttonHeight = renderableComponent.Bounds.w;

                    glm::vec2 textDimensions = font->CalculateTextDimensions(renderableTextComponent.Text, scale);
                    x = fmax((buttonWidth - textDimensions.x) * 0.5f, 20.0f);
                    y = 30.0f + fmax((buttonHeight - textDimensions.y) * 0.5f, 20.0f);

                    auto triangleCount = 0;
                    // TODO: buffer pooling
                    for (const char &c: renderableTextComponent.Text) {
                        const auto glyph = glyphs[c];

                        if (glyph.planeBounds == glm::vec4(0.0f) || glyph.atlasBounds == glm::vec4(0.0f)) {
                            x += glyph.advance * scale;
                            continue;
                        }

                        float left = x + glyph.planeBounds.x * scale;
                        float bottom = y + glyph.planeBounds.y * scale;
                        float right = x + glyph.planeBounds.z * scale;
                        float top = y + glyph.planeBounds.w * scale;

                        float textureLeft = glyph.atlasBounds.x / static_cast<float>(fontTexture->GetWidth());
                        float textureBottom = glyph.atlasBounds.y / static_cast<float>(fontTexture->GetHeight());
                        float textureRight = glyph.atlasBounds.z / static_cast<float>(fontTexture->GetWidth());
                        float textureTop = glyph.atlasBounds.w / static_cast<float>(fontTexture->GetHeight());

                        float vertices[6][4] = {{left,  top,    textureLeft,  textureTop},
                                                {left,  bottom, textureLeft,  textureBottom},
                                                {right, bottom, textureRight, textureBottom},
                                                {left,  top,    textureLeft,  textureTop},
                                                {right, bottom, textureRight, textureBottom},
                                                {right, top,    textureRight, textureTop}};

                        vbo_.SetSubData(sizeof(float) * 4 * 3 * triangleCount, sizeof(vertices), vertices);

                        triangleCount += 2;
                        x += glyph.advance * scale;
                    }

                    vbo_.VertexAttribPointer(0, 2, GL_FLOAT, sizeof(float) * 4, 0);
                    vbo_.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(float) * 4, sizeof(float) * 2);
                    vao_.Draw(GL_TRIANGLES, 0, triangleCount * 3);
                }
            }
        }

        endFrame_();
    }

    void RenderSystem::startFrame_() {
        const auto size = window_->GetSize();
        glViewport(0, 0, size.x, size.y);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderSystem::endFrame_() {
        // Swap buffers and poll events
        window_->Swap();
    }
}
