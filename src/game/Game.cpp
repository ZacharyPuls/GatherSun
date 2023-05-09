//
// Created by zach on 2023-04-22.
//

#include "ui/Text.h"
#include "system/RenderSystem.h"
#include "system/EventSystem.h"
#include "ui/PauseMenu.h"
#include "Game.h"
#include "Window.h"
#include "Util.h"
#include "entity/Component.h"
#include "Serializer.h"

namespace gathersun::game {

    Game::Game() {

        auto window = std::make_shared<Window>("GatherSun", 960, 720, false);
        registry_.ctx().emplace<std::shared_ptr<Window>>(window);

        auto firaCodeMedium = std::make_shared<ui::MSDFFont>("res/font/FiraCode-Medium.json", "res/font/FiraCode-Medium.bmp");
        registry_.ctx().emplace_as<std::shared_ptr<ui::MSDFFont>>(entt::hashed_string(firaCodeMedium->GetName().c_str()), firaCodeMedium);

        auto pixelArt = std::make_shared<ui::MSDFFont>("res/font/PixelArt.json", "res/font/PixelArt.bmp");
        registry_.ctx().emplace_as<std::shared_ptr<ui::MSDFFont>>(entt::hashed_string(pixelArt->GetName().c_str()), pixelArt);

        auto pauseMenu = registry_.create();
        registry_.emplace<std::shared_ptr<ui::Menu>>(pauseMenu, std::make_shared<ui::PauseMenu>(window->GetSize(), registry_))->SetActive(true);


        /*ui::RenderableTextComponent renderableText = {
                .Color = glm::vec3(1.0f, 1.0f, 1.0f),
                .Text = "The quick brown fox jumped over the lazy dogs!",
                .Font = firaCodeMedium
        };

        auto textComponent = registry_.create();

        auto renderableTextComponent = registry_.emplace<ui::RenderableTextComponent>(textComponent,
                                                                                      renderableText);

        TransformComponent transform = {
                .Translation = glm::vec3(100.0f, 100.0f, 0.0f),
                .Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                .Scale = glm::vec3(1.0f, 1.0f, 1.0f)
        };

        auto transformComponent = registry_.emplace<TransformComponent>(textComponent, transform);

        RotatingEffectComponent rotationEffect = {
                .Speed = 0.01f,
                .Axis = glm::vec3(0.0f, 0.0f, 1.0f)
        };

        auto rotatingEffectComponent = registry.emplace<RotatingEffectComponent>(text, rotationEffect);

        ColorShiftEffectComponent colorShiftEffect = {
                .Speed = 1.0f,
                .StartColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                .EndColor = glm::vec4(1.0f, 1.0f, 0.01f, 1.0f),
                .Progress = 0.0f
        };

        auto colorShiftEffectComponent = registry_.emplace<ColorShiftEffectComponent>(textComponent,
                                                                                      colorShiftEffect);

        auto testPlayerSpritesheetTexture = registry_.create();

        auto testPlayerSprite = registry_.create();*/

        renderSystem_ = std::make_shared<system::RenderSystem>(registry_);
        eventSystem_ = std::make_shared<system::EventSystem>(registry_);
    }

    Game::~Game() {
        // Note: maybe with entities (textures, fonts, etc), just the name/filename is stored in the components (text, menu, etc),
        //       and on load, the entities are dynamically recreated.
        Serializer<ui::RenderableTextComponent, entity::TransformComponent, entity::ColorShiftEffectComponent> serializer("save.json");
        serializer.Serialize(registry_);
    }
}
