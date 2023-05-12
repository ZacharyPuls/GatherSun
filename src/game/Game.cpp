//
// Created by zach on 2023-04-22.
//

#include "Game.h"

#include "ui/Text.h"
#include "system/RenderSystem.h"
#include "system/EventSystem.h"
#include "ui/PauseMenu.h"
#include "Window.h"
#include "entity/Object.h"
#include "render/Renderable.h"
#include "entity/Movable.h"

namespace gathersun::game {

    Game::Game() {
        window_ = std::make_shared<Window>("GatherSun", 960, 720, false);
        scene_ = std::make_shared<scene::Scene>();

/*        auto &firaCodeMedium = scene_->AddObject("Fira Code Medium");
        firaCodeMedium.AddComponent<ui::MSDFFont>("res/font/FiraCode-Medium.json", "res/font/FiraCode-Medium.bmp");*/

        auto &pixelArt = scene_->AddObject("Pixel Art");
        pixelArt.AddComponent<ui::MSDFFont>("res/font/PixelArt.json", "res/font/PixelArt.bmp");

        auto &pauseMenu = scene_->AddObject("PauseMenu");
        auto &pauseMenuComponent = pauseMenu.AddComponent<ui::Menu>(ui::CreatePauseMenu(scene_.get(), "PauseMenu", "res/menu/ui-elements.json", window_->GetSize()));
        pauseMenuComponent.SetActive(true);

        uiTexture_ = std::make_shared<render::Texture2D>("res/menu/ui-elements.bmp", true);

        auto &player = scene_->AddObject("Player");
        player.AddComponent<render::Position>(glm::vec2{100.0f, 100.0f});
        player.AddComponent<entity::Movable>();
        player.AddComponent<render::Renderable>("res/sprite/test-spritesheet.png", "res/sprite/test-spritesheet.json");
        render::Animatable animatable;
        std::ifstream spritesheetMetadataFile("res/sprite/test-spritesheet.json");
        nlohmann::json spritesheetMetadata = nlohmann::json::parse(spritesheetMetadataFile);
        spritesheetMetadata.at("animation").get_to(animatable);
        player.AddComponent<render::Animatable>(animatable);
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

        renderSystem_ = std::make_shared<system::RenderSystem>(scene_.get(), window_.get(), &state_);
        eventSystem_ = std::make_shared<system::EventSystem>(scene_.get(), window_.get(), &state_);
    }

    Game::~Game() {
        // Note: maybe with entities (textures, fonts, etc), just the name/filename is stored in the components (text, menu, etc),
        //       and on load, the entities are dynamically recreated.
//        Serializer<ui::RenderableTextComponent, entity::TransformComponent, entity::ColorShiftEffectComponent> serializer("save.json");
//        serializer.Serialize();
    }
}
