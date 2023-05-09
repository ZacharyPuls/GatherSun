//
// Created by zach on 2023-04-29.
//

#ifndef GATHERSUN_UI_TEXT_H
#define GATHERSUN_UI_TEXT_H

#include "Core.h"
#include "Font.h"
#include "Util.h"

namespace gathersun::ui {

    struct RenderableTextComponent {
        glm::vec3 Color = glm::vec3(1.0f);
        float Scale = 1.0f;
        std::string Text;
        std::shared_ptr<MSDFFont> Font;

        friend void to_json(nlohmann::json &nlohmann_json_j, const RenderableTextComponent &nlohmann_json_t) {
            nlohmann_json_j["Color"] = nlohmann_json_t.Color;
            nlohmann_json_j["Scale"] = nlohmann_json_t.Scale;
            nlohmann_json_j["Text"] = nlohmann_json_t.Text;
            nlohmann_json_j["Font"] = nlohmann_json_t.Font->GetName();
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, RenderableTextComponent &nlohmann_json_t) {
            nlohmann_json_j.at("Color").get_to(nlohmann_json_t.Color);
            nlohmann_json_j.at("Scale").get_to(nlohmann_json_t.Scale);
            nlohmann_json_j.at("Text").get_to(nlohmann_json_t.Text);
        }
    };
}

#endif // GATHERSUN_UI_TEXT_H
