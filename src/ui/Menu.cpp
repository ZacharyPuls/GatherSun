//
// Created by zach on 2023-04-23.
//

#include "Menu.h"
#include "Text.h"
#include "entity/Object.h"
#include "render/Renderable.h"
#include "ui/Activatable.h"

#include <utility>

namespace gathersun::ui {

    Menu::Menu(scene::Scene *scene, std::string name, const std::string &uiElementsPath) : scene_(scene),
                                                                                           name_(std::move(name)) {
        std::ifstream uiElementsFile(uiElementsPath);
        uiElements_ = nlohmann::json::parse(uiElementsFile).get<UIElements>();
        uiTexture_ = std::make_shared<render::Texture2D>("res/menu/ui-elements.bmp", true);
    }

    bool Menu::IsActive() const {
        return active_;
    }

    void Menu::SetActive(bool active) {
        active_ = active;
    }

    const std::string &Menu::GetFont() const {
        return font_;
    }

    void Menu::SetFont(const std::string &font) {
        font_ = font;
    }

    const std::string &Menu::GetName() const {
        return name_;
    }

    void Menu::SetName(const std::string &name) {
        name_ = name;
    }

    const std::vector<entt::entity> &Menu::GetElements() const {
        return elements_;
    }

    void Menu::AddButton(glm::ivec2 position, glm::ivec2 size, std::function<void(void)> onClick, const std::string &text) {
        auto &button = scene_->AddObject(text);
        auto &font = scene_->GetObject(font_);
        auto &fontComponent = font.GetComponent<ui::MSDFFont>();
        button.AddComponent<render::Renderable>(render::Renderable(uiTexture_, {0.0f, 0.0f, size.x, size.y},
                                                                   uiElements_.ObjectTypes["button"].Inactive));
        button.AddComponent<render::Position>(render::Position{.Position = position});
        button.AddComponent<ui::Activatable>(
                ui::Activatable{.ActiveTexcoords = uiElements_.ObjectTypes["button"].Active, .OnClick = std::move(
                        onClick), .Active = false});
        button.AddComponent<ui::RenderableTextComponent>(ui::RenderableTextComponent{.Color = glm::vec3(
                1.0f), .Scale = 32.0f, .Text = text, .Font = std::make_shared<ui::MSDFFont>(fontComponent)});
        elements_.push_back(static_cast<entt::entity>(button));
    }

    void Menu::AddListBox(glm::ivec2 position, glm::ivec2 size, std::function<void(const std::string &)> onSelect,
                          const std::vector<std::string> &values) {
        auto &
    }

    void Menu::Resize(const glm::ivec2 size) {
        const auto &components = GetElements();
        // Iterate backwards, as the buttons are expected to be FIFO
        auto i = 0;
        std::for_each(components.rbegin(), components.rend(), [&](const auto &component) {
            auto &element = scene_->GetObject(component);
            auto &renderableComponent = element.template GetComponent<render::Renderable>();
            auto x = static_cast<int>((size.x - renderableComponent.Bounds.z) * 0.5f);
            // TODO: Padding
            auto y = static_cast<int>((size.y - (renderableComponent.Bounds.w * components.size())) * 0.5f) +
                     (i * renderableComponent.Bounds.w);
            // TODO: Element resizing to fit window scale/aspect ratio
            auto &positionComponent = element.template GetComponent<render::Position>();
            positionComponent.Position.x = x;
            positionComponent.Position.y = y;
            ++i;
        });
    }
}
