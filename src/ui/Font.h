//
// Created by zach on 2021-09-18.
//

#ifndef GATHERSUN_UI_FONT_H
#define GATHERSUN_UI_FONT_H

#include "Core.h"
#include "render/VertexArray.h"
#include "render/VertexBuffer.h"
#include "render/Shader.h"
#include "render/Texture.h"

#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fstream>

namespace gathersun::ui {

    class Font {
    public:
        virtual ~Font() = default;
        virtual const render::Texture& GetTexture() const = 0;
        virtual const std::string &GetName() const = 0;
    };

    class MSDFFont : public Font {
    public:
        struct Atlas {
            std::string type;
            uint32_t distanceRange;
            float size;
            uint32_t width;
            uint32_t height;
            bool yOriginBottom;
        };

        struct Metrics {
            uint32_t emSize;
            float lineHeight;
            float ascender;
            float descender;
            float underlineY;
            float underlineThickness;
        };

        struct Glyph {
            uint32_t unicode;
            float advance;
            glm::vec4 planeBounds;
            glm::vec4 atlasBounds;
        };

        explicit MSDFFont(const std::string &fontMetadataPath, const std::string &fontAtlasPath);

        const render::Texture2D &GetTexture() const override;

        const Atlas &GetAtlas() const;

        const std::string &GetName() const override;

        const Metrics &GetMetrics() const;

        const std::map<uint32_t, Glyph> &GetGlyphs() const;

        glm::vec2 CalculateTextDimensions(const std::string &text, const float scale) const;

        friend void to_json(nlohmann::json &nlohmann_json_j, const MSDFFont &font);

        friend void from_json(const nlohmann::json &nlohmann_json_j, MSDFFont &nlohmann_json_t);

    private:
        render::Texture2D texture_;
        Atlas atlas_;
        std::string name_;
        Metrics metrics_;
        std::map<uint32_t, Glyph> glyphs_;
    };

    void to_json(nlohmann::json &j, const MSDFFont::Atlas &atlas);

    void from_json(const nlohmann::json &j, MSDFFont::Atlas &atlas);

    void to_json(nlohmann::json &j, const MSDFFont::Metrics &metrics);

    void from_json(const nlohmann::json &j, MSDFFont::Metrics &metrics);

    void to_json(nlohmann::json &j, const MSDFFont::Glyph &glyph);

    void from_json(const nlohmann::json &j, MSDFFont::Glyph &glyph);
}

#endif // GATHERSUN_UI_FONT_H
