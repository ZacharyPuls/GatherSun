//
// Created by zach on 2021-09-28.
//

#include "RenderSystem.h"
#include "entity/Component.h"
#include "ui/Menu.h"
#include "event/EventManager.h"
#include "entity/Entity.h"
#include "entity/Player.h"
#include "render/Shader.h"

#include <iostream>

namespace gathersun::system {

    RenderSystem::RenderSystem(entt::registry &registry) : System(registry), defaultShader_("res/shader/default.glsl"),
                                                           textShader_("res/shader/text.glsl"),
                                                           vbo_(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
                                                           uiTexture_("res/menu/ui-elements.bmp", true) {
        glEnable(GL_MULTISAMPLE);
        scene_.SetCamera(scene::Camera{});
//        scene_.GetCamera().SetPosition(glm::vec3(0.0f));
        auto playerSprite = Sprite{"res/sprite/test-spritesheet.png", "res/sprite/test-spritesheet.json"};
        auto playerEntity = entity::Player(playerSprite, glm::vec2(100.0f, 100.0f));
        scene_.AddPlayer(registry_, playerEntity);
    }

    void RenderSystem::Run(double dt) {
        registry_.view<ui::RenderableTextComponent, entity::TransformComponent, entity::RotatingEffectComponent>().each(
                [dt](auto &text, auto &transform, auto &rotatingEffect) {
                    transform.Rotation = glm::rotate(transform.Rotation, rotatingEffect.Speed * static_cast<float>(dt),
                                                     rotatingEffect.Axis);
                });

        registry_.view<ui::RenderableTextComponent, entity::ColorShiftEffectComponent>().each(
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

        auto window = registry_.ctx().get<std::shared_ptr<game::Window>>();
        startFrame_(window);

        vao_.Bind();
        vbo_.Bind();
        // TODO: actually size this properly
        vbo_.SetData(sizeof(float) * 6 * 4 * 256, nullptr);

        auto width = window->GetSize().x;
        auto height = window->GetSize().y;

        auto &camera = scene_.GetCamera();
        camera.OrthographicProjection(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
        const auto projection = camera.GetProjectionMatrix();

        auto menuView = registry_.view<std::shared_ptr<ui::Menu>>();
        // TODO: do we really want to do one draw call per type (menu element, text, etc)?

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // TODO: separate out these draw passes into objects that can be applied/enabled/disabled to a scene at will

        // Game draw pass
        {
            defaultShader_.Bind();
            auto triangleCount = 0;
            for (const auto &entityId: scene_.GetEntities()) {
                auto &entity = registry_.get<entity::Entity>(entityId);
                const auto transform = entity::TransformComponent{glm::vec3(entity.GetPosition(), 0.0f), {},
                                                          glm::vec3(1.0f)};
                defaultShader_.UniformMat4(0, projection * transform.GetModelMatrix());
                entity.GetSprite().GetTexture().Bind();
                defaultShader_.UniformTexture(1, 0);

                auto spriteSize = entity.GetSprite().GetSize();
                float left = spriteSize.x;
                float bottom = spriteSize.y;
                float right = spriteSize.z;
                float top = spriteSize.w;

                auto spriteTexcoords = entity.GetSprite().GetTexcoords();
                float textureLeft = spriteTexcoords.x;
                float textureBottom = spriteTexcoords.y;
                float textureRight = spriteTexcoords.z;
                float textureTop = spriteTexcoords.w;

                float vertices[6][4] = {{left,  top,    textureLeft,  textureTop},
                                        {left,  bottom, textureLeft,  textureBottom},
                                        {right, bottom, textureRight, textureBottom},
                                        {left,  top,    textureLeft,  textureTop},
                                        {right, bottom, textureRight, textureBottom},
                                        {right, top,    textureRight, textureTop}};

                vbo_.SetSubData(sizeof(float) * 4 * 3 * triangleCount, sizeof(vertices), vertices);
                triangleCount += 2;
            }
            auto &entity = registry_.ctx().get<entity::Player>();
            const auto transform = entity::TransformComponent{glm::vec3(entity.GetPosition(), 0.0f), {},
                                                      glm::vec3(1.0f)};
            defaultShader_.UniformMat4(0, projection * transform.GetModelMatrix());
            entity.GetSprite().GetTexture().Bind();
            defaultShader_.UniformTexture(1, 0);

            auto spriteSize = entity.GetSprite().GetSize();
            float left = spriteSize.x;
            float bottom = spriteSize.y;
            float right = spriteSize.z;
            float top = spriteSize.w;

            auto spriteTexcoords = entity.GetSprite().GetTexcoords();
            float textureLeft = spriteTexcoords.x;
            float textureBottom = spriteTexcoords.y;
            float textureRight = spriteTexcoords.z;
            float textureTop = spriteTexcoords.w;

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
            glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
        }

        // UI draw pass
        defaultShader_.Bind();
        for (auto &menuEntity: menuView) {
            auto menu = menuView.get<std::shared_ptr<ui::Menu>>(menuEntity);
            if (menu->IsActive()) {
                for (auto component: menu->GetComponents()) {
                    auto triangleCount = 0;
                    auto buttonComponent = registry_.get<ui::ButtonComponent>(component);
                    // TODO: Do we want to move ButtonComponent::Position to a TransformComponent?
                    const auto transform = entity::TransformComponent{glm::vec3(buttonComponent.Position, 0.0f), {},
                                                              glm::vec3(1.0f)};

                    defaultShader_.UniformMat4(0, projection * transform.GetModelMatrix());
                    uiTexture_.Bind();
                    defaultShader_.UniformTexture(1, 0);

                    auto buttonTexcoords = buttonComponent.Active ? buttonComponent.ActiveTexcoords
                                                                  : buttonComponent.InactiveTexcoords;
                    float left = buttonComponent.PlaneBounds.x;
                    float bottom = buttonComponent.PlaneBounds.y;
                    float right = buttonComponent.PlaneBounds.z;
                    float top = buttonComponent.PlaneBounds.w;

                    float textureLeft = buttonTexcoords.x / static_cast<float>(256);
                    float textureBottom = buttonTexcoords.y / static_cast<float>(256);
                    float textureRight = buttonTexcoords.z / static_cast<float>(256);
                    float textureTop = buttonTexcoords.w / static_cast<float>(256);

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
                    glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
                }
            }
        }

        // Text draw pass
        textShader_.Bind();
        vbo_.SetData(sizeof(float) * 6 * 4 * 256, nullptr);
        for (auto &menuEntity: menuView) {
            auto menu = menuView.get<std::shared_ptr<ui::Menu>>(menuEntity);
            if (menu->IsActive()) {
                for (auto component: menu->GetComponents()) {
                    auto [buttonComponent, renderableTextComponent] = registry_.get<ui::ButtonComponent, ui::RenderableTextComponent>(
                            component);
                    const auto &font = renderableTextComponent.Font;
                    const auto &fontTexture = font->GetTexture();
                    const auto transform = entity::TransformComponent{glm::vec3(buttonComponent.Position, 0.0f), {},
                                                              glm::vec3(1.0f)};

                    textShader_.UniformMat4(0, projection * transform.GetModelMatrix());
                    fontTexture.Bind();
                    textShader_.UniformTexture(1, 0);
                    textShader_.UniformVec3(2, renderableTextComponent.Color);
                    const auto &atlas = font->GetAtlas();

                    // TODO: is this copy really worth it to avoid std::map::at()?
                    auto glyphs = font->GetGlyphs();

                    float x = 0.0f;
                    float y = 0.0f;
                    // float scale = atlas.size;
                    float scale = renderableTextComponent.Scale;

                    float buttonWidth = buttonComponent.PlaneBounds.z;
                    float buttonHeight = buttonComponent.PlaneBounds.w;

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

                        float textureLeft = glyph.atlasBounds.x / static_cast<float>(fontTexture.GetWidth());
                        float textureBottom = glyph.atlasBounds.y / static_cast<float>(fontTexture.GetHeight());
                        float textureRight = glyph.atlasBounds.z / static_cast<float>(fontTexture.GetWidth());
                        float textureTop = glyph.atlasBounds.w / static_cast<float>(fontTexture.GetHeight());

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
                    glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
                }
            }
        }

        endFrame_(window);
    }

    void RenderSystem::startFrame_(const std::shared_ptr<game::Window> &window) {
        const auto size = window->GetSize();
        glViewport(0, 0, size.x, size.y);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderSystem::endFrame_(const std::shared_ptr<game::Window> &window) {
        // Swap buffers and poll events
        window->Swap();
    }
}
