//
// Created by zach on 2023-04-23.
//

#include "Menu.h"

#include <utility>
#include "Text.h"

namespace gathersun::ui {

    Menu::Menu(std::string name, const std::string &uiElementsPath) : name_(std::move(name)) {
        std::ifstream uiElementsFile(uiElementsPath);
        uiElements_ = nlohmann::json::parse(uiElementsFile).get<UIElements>();
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

    const std::vector<entt::entity> &Menu::GetComponents() const {
        return components_;
    }

    void Menu::AddButton(entt::registry &registry, glm::ivec2 position, glm::ivec2 size,
                         std::function<void(entt::registry &)> onClick, const std::string &text) {
        auto button = registry.create();
        auto buttonPlaneBounds = uiElements_.ObjectTypes["button"].PlaneBounds;
        auto buttonInactiveTexcoords = uiElements_.ObjectTypes["button"].Inactive;
        auto buttonActiveTexcoords = uiElements_.ObjectTypes["button"].Active;
        registry.emplace<ButtonComponent>(button, ButtonComponent{position, size,
                                                                  {buttonPlaneBounds.left, buttonPlaneBounds.bottom,
                                                                   buttonPlaneBounds.right, buttonPlaneBounds.top},
                                                                  {buttonInactiveTexcoords.left,
                                                                   buttonInactiveTexcoords.bottom,
                                                                   buttonInactiveTexcoords.right,
                                                                   buttonInactiveTexcoords.top},
                                                                  {buttonActiveTexcoords.left,
                                                                   buttonActiveTexcoords.bottom,
                                                                   buttonActiveTexcoords.right,
                                                                   buttonActiveTexcoords.top}, std::move(onClick),
                                                                  false});
        auto font = registry.ctx().find<std::shared_ptr<MSDFFont>>(entt::hashed_string(font_.c_str()));
        // TODO: Do we really want to de-reference a pointer to a shared_ptr? This seems odd.
        registry.emplace<RenderableTextComponent>(button, RenderableTextComponent{glm::vec3(1.0f), 32.0f, text, *font});
        components_.push_back(button);
    }
}
