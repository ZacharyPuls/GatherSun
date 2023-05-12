//
// Created by zach on 2023-05-07.
//

#ifndef GATHERSUN_RENDER_RENDERABLE_H
#define GATHERSUN_RENDER_RENDERABLE_H

#include "Core.h"
#include "Texture.h"
#include "Util.h"

#include <fstream>

namespace gathersun::render {

    struct Position {
        glm::vec2 Position;
    };

    struct Renderable {
        Renderable() = default;

        Renderable(const std::shared_ptr<Texture2D> &texture, const glm::vec4 &bounds, const glm::vec4 &texcoords,
                   bool visible = true) : Texture(texture), Bounds(bounds), Texcoords(texcoords), Visible(visible) {}

        Renderable(const std::string &spriteSheetFilename, const std::string &spriteSheetMetadataFilename) : Texture(
                std::make_shared<Texture2D>(spriteSheetFilename, true)) {
            std::ifstream spriteSheetMetadataFile(spriteSheetMetadataFilename);
            nlohmann::json spriteSheetMetadata = nlohmann::json::parse(spriteSheetMetadataFile);
            auto width = 0;
            auto height = 0;
            spriteSheetMetadata.at("atlas").at("width").get_to(width);
            spriteSheetMetadata.at("atlas").at("height").get_to(height);
            spriteSheetMetadata.at("bounds").get_to(Bounds);
            Texcoords = {0.0f, 0.0f, 1.0f, 1.0f};
        }

        std::shared_ptr<Texture2D> Texture;
        glm::vec4 Bounds;
        glm::vec4 Texcoords;
        bool Visible = true;
    };
}

#endif // GATHERSUN_RENDER_RENDERABLE_H
