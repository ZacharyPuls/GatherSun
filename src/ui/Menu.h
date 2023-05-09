//
// Created by zach on 2023-04-23.
//

#ifndef GATHERSUN_UI_MENU_H
#define GATHERSUN_UI_MENU_H

#include "Core.h"

namespace gathersun::ui {

    // TODO: naming
    struct UIElements {
        struct Atlas {
            uint32_t Width;
            uint32_t Height;
            bool yOriginBottom;

            friend void to_json(nlohmann::json &nlohmann_json_j, const Atlas &nlohmann_json_t) {
                nlohmann_json_j["width"] = nlohmann_json_t.Width;
                nlohmann_json_j["weight"] = nlohmann_json_t.Height;
                nlohmann_json_j["yOrigin"] = nlohmann_json_t.yOriginBottom ? "bottom" : "top";
            }

            friend void from_json(const nlohmann::json &nlohmann_json_j, Atlas &nlohmann_json_t) {
                nlohmann_json_j.at("width").get_to(nlohmann_json_t.Width);
                nlohmann_json_j.at("height").get_to(nlohmann_json_t.Height);
                std::string yOrigin;
                nlohmann_json_j.at("yOrigin").get_to(yOrigin);
                nlohmann_json_t.yOriginBottom = yOrigin == "bottom";
            }
        };

        struct ObjectType {
            struct PlaneBounds {
                float left;
                float bottom;
                float right;
                float top;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlaneBounds, left, bottom, right, top);
            };

            struct AtlasBounds {
                float left;
                float bottom;
                float right;
                float top;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(AtlasBounds, left, bottom, right, top);
            };

            std::string Name;
            PlaneBounds PlaneBounds;
            AtlasBounds Inactive;
            AtlasBounds Active;

            friend void to_json(nlohmann::json &nlohmann_json_j, const ObjectType &nlohmann_json_t) {
                nlohmann_json_j["name"] = nlohmann_json_t.Name;
                nlohmann_json_j["planeBounds"] = nlohmann_json_t.PlaneBounds;
                nlohmann_json_j["atlasBounds"]["inactive"] = nlohmann_json_t.Inactive;
                nlohmann_json_j["atlasBounds"]["active"] = nlohmann_json_t.Active;
            }

            friend void from_json(const nlohmann::json &nlohmann_json_j, ObjectType &nlohmann_json_t) {
                nlohmann_json_j.at("name").get_to(nlohmann_json_t.Name);
                nlohmann_json_j.at("planeBounds").get_to(nlohmann_json_t.PlaneBounds);
                nlohmann_json_j.at("atlasBounds").at("inactive").get_to(nlohmann_json_t.Inactive);
                nlohmann_json_j.at("atlasBounds").at("active").get_to(nlohmann_json_t.Active);
            }
        };

        Atlas Atlas;
        std::map<std::string, ObjectType> ObjectTypes;

        friend void to_json(nlohmann::json &nlohmann_json_j, const UIElements &nlohmann_json_t) {
            nlohmann_json_j["atlas"] = nlohmann_json_t.Atlas;
            auto keys = std::views::keys(nlohmann_json_t.ObjectTypes);
            nlohmann_json_j["objectTypes"] = std::vector<ObjectType>{keys.begin(), keys.end()};
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, UIElements &nlohmann_json_t) {
            nlohmann_json_j.at("atlas").get_to(nlohmann_json_t.Atlas);
            std::vector<ObjectType> objectTypes = nlohmann_json_j.at("objectTypes");
            std::transform(objectTypes.begin(), objectTypes.end(),
                           std::inserter(nlohmann_json_t.ObjectTypes, nlohmann_json_t.ObjectTypes.end()),
                           [](const ObjectType &objectType) { return std::make_pair(objectType.Name, objectType); });
        }
    };

    struct ButtonComponent {
        glm::ivec2 Position;
        glm::ivec2 Size;
        glm::vec4 PlaneBounds;
        glm::vec4 InactiveTexcoords;
        glm::vec4 ActiveTexcoords;
        std::function<void(entt::registry&)> OnClick;
        bool Active = false;
    };

    class Menu {
    public:
        Menu() = default;

        Menu(std::string name, const std::string &uiElementsPath);

        ~Menu() = default;

        bool IsActive() const;

        void SetActive(bool active);

        const std::string &GetFont() const;

        void SetFont(const std::string &font);

        const std::string &GetName() const;

        void SetName(const std::string &name);

        const std::vector<entt::entity> &GetComponents() const;

        void AddButton(entt::registry &registry, glm::ivec2 position, glm::ivec2 size,
                       std::function<void(entt::registry &)> onClick, const std::string &text);

        virtual void Resize(const glm::ivec2 size, entt::registry &registry) = 0;

    protected:
        UIElements uiElements_;

    private:
        bool active_ = false;
        std::string font_ = "Pixel Art";
        std::string name_;
        std::vector<entt::entity> components_;
    };
}

#endif // GATHERSUN_UI_MENU_H
