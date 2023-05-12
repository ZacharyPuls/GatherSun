//
// Created by zach on 2023-04-23.
//

#ifndef GATHERSUN_UI_MENU_H
#define GATHERSUN_UI_MENU_H

#include "Core.h"
#include "scene/Scene.h"
#include "render/Texture.h"

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
            std::string Name;
            glm::vec4 PlaneBounds;
            glm::vec4 Inactive;
            glm::vec4 Active;

            friend void to_json(nlohmann::json &nlohmann_json_j, const ObjectType &nlohmann_json_t) {
                nlohmann_json_j["name"] = nlohmann_json_t.Name;
                nlohmann_json_j["planeBounds"]["left"] = nlohmann_json_t.PlaneBounds.x;
                nlohmann_json_j["planeBounds"]["bottom"] = nlohmann_json_t.PlaneBounds.y;
                nlohmann_json_j["planeBounds"]["right"] = nlohmann_json_t.PlaneBounds.z;
                nlohmann_json_j["planeBounds"]["top"] = nlohmann_json_t.PlaneBounds.w;
                nlohmann_json_j["atlasBounds"]["inactive"]["left"] = nlohmann_json_t.Inactive.x;
                nlohmann_json_j["atlasBounds"]["inactive"]["bottom"] = nlohmann_json_t.Inactive.y;
                nlohmann_json_j["atlasBounds"]["inactive"]["right"] = nlohmann_json_t.Inactive.z;
                nlohmann_json_j["atlasBounds"]["inactive"]["top"] = nlohmann_json_t.Inactive.w;
                nlohmann_json_j["atlasBounds"]["active"]["left"] = nlohmann_json_t.Active.x;
                nlohmann_json_j["atlasBounds"]["active"]["bottom"] = nlohmann_json_t.Active.y;
                nlohmann_json_j["atlasBounds"]["active"]["right"] = nlohmann_json_t.Active.z;
                nlohmann_json_j["atlasBounds"]["active"]["top"] = nlohmann_json_t.Active.w;
            }

            friend void from_json(const nlohmann::json &nlohmann_json_j, ObjectType &nlohmann_json_t) {
                nlohmann_json_j.at("name").get_to(nlohmann_json_t.Name);
                nlohmann_json_j.at("planeBounds").at("left").get_to(nlohmann_json_t.PlaneBounds.x);
                nlohmann_json_j.at("planeBounds").at("bottom").get_to(nlohmann_json_t.PlaneBounds.y);
                nlohmann_json_j.at("planeBounds").at("right").get_to(nlohmann_json_t.PlaneBounds.z);
                nlohmann_json_j.at("planeBounds").at("top").get_to(nlohmann_json_t.PlaneBounds.w);
                nlohmann_json_j.at("atlasBounds").at("inactive").at("left").get_to(nlohmann_json_t.Inactive.x);
                nlohmann_json_j.at("atlasBounds").at("inactive").at("bottom").get_to(nlohmann_json_t.Inactive.y);
                nlohmann_json_j.at("atlasBounds").at("inactive").at("right").get_to(nlohmann_json_t.Inactive.z);
                nlohmann_json_j.at("atlasBounds").at("inactive").at("top").get_to(nlohmann_json_t.Inactive.w);
                nlohmann_json_j.at("atlasBounds").at("active").at("left").get_to(nlohmann_json_t.Active.x);
                nlohmann_json_j.at("atlasBounds").at("active").at("bottom").get_to(nlohmann_json_t.Active.y);
                nlohmann_json_j.at("atlasBounds").at("active").at("right").get_to(nlohmann_json_t.Active.z);
                nlohmann_json_j.at("atlasBounds").at("active").at("top").get_to(nlohmann_json_t.Active.w);
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

    class Menu {
    public:
        Menu() = default;

        Menu(scene::Scene *scene, std::string name, const std::string &uiElementsPath);

        ~Menu() = default;

        bool IsActive() const;

        void SetActive(bool active);

        const std::string &GetFont() const;

        void SetFont(const std::string &font);

        const std::string &GetName() const;

        void SetName(const std::string &name);

        const std::vector<entt::entity> &GetElements() const;

        void
        AddButton(glm::ivec2 position, glm::ivec2 size, std::function<void(void)> onClick, const std::string &text);

        void AddListBox(glm::ivec2 position, glm::ivec2 size, std::function<void(const std::string &)> onSelect,
                        const std::vector<std::string> &values);

        void Resize(glm::ivec2 size);

    protected:
        UIElements uiElements_;
        scene::Scene *scene_;
        std::shared_ptr<render::Texture2D> uiTexture_;
    private:
        bool active_ = false;
        std::string font_ = "Pixel Art";
        std::string name_;
        std::vector<entt::entity> elements_;
    };
}

#endif // GATHERSUN_UI_MENU_H
