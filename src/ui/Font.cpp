//
// Created by zach on 2021-09-18.
//

#include "Font.h"
#include "render/Texture.h"
#include "render/Shader.h"

#include <iostream>

namespace gathersun::ui {

    void to_json(nlohmann::json &j, const MSDFFont::Atlas &atlas) {
        j = nlohmann::json{{"type",          atlas.type},
                           {"distanceRange", atlas.distanceRange},
                           {"size",          atlas.size},
                           {"width",         atlas.width},
                           {"height",        atlas.height},
                           {"yOriginBottom", atlas.yOriginBottom}};
    }

    void from_json(const nlohmann::json &j, MSDFFont::Atlas &atlas) {
        j.at("type").get_to(atlas.type);
        j.at("distanceRange").get_to(atlas.distanceRange);
        j.at("size").get_to(atlas.size);
        j.at("width").get_to(atlas.width);
        j.at("height").get_to(atlas.height);
        j.at("yOrigin") == "bottom" ? atlas.yOriginBottom = true : atlas.yOriginBottom = false;
    }

    void to_json(nlohmann::json &j, const MSDFFont::Metrics &metrics) {
        j = nlohmann::json{{"emSize",             metrics.emSize},
                           {"lineHeight",         metrics.lineHeight},
                           {"ascender",           metrics.ascender},
                           {"descender",          metrics.descender},
                           {"underlineY",         metrics.underlineY},
                           {"underlineThickness", metrics.underlineThickness}};
    }

    void from_json(const nlohmann::json &j, MSDFFont::Metrics &metrics) {
        j.at("emSize").get_to(metrics.emSize);
        j.at("lineHeight").get_to(metrics.lineHeight);
        j.at("ascender").get_to(metrics.ascender);
        j.at("descender").get_to(metrics.descender);
        j.at("underlineY").get_to(metrics.underlineY);
        j.at("underlineThickness").get_to(metrics.underlineThickness);
    }

    void to_json(nlohmann::json &j, const MSDFFont::Glyph &glyph) {
        j = nlohmann::json{{"unicode",     glyph.unicode},
                           {"advance",     glyph.advance},
                           {"planeBounds", {{"left", glyph.planeBounds.x}, {"bottom", glyph.planeBounds.y}, {"right", glyph.planeBounds.z}, {"top", glyph.planeBounds.w}}},
                           {"atlasBounds", {{"left", glyph.atlasBounds.x}, {"bottom", glyph.atlasBounds.y}, {"right", glyph.atlasBounds.z}, {"top", glyph.atlasBounds.w}}}};
    }

    void from_json(const nlohmann::json &j, MSDFFont::Glyph &glyph) {
        j.at("unicode").get_to(glyph.unicode);
        j.at("advance").get_to(glyph.advance);
        if (j.contains("planeBounds")) {
            j.at("planeBounds").at("left").get_to(glyph.planeBounds.x);
            j.at("planeBounds").at("bottom").get_to(glyph.planeBounds.y);
            j.at("planeBounds").at("right").get_to(glyph.planeBounds.z);
            j.at("planeBounds").at("top").get_to(glyph.planeBounds.w);
        }
        if (j.contains("atlasBounds")) {
            j.at("atlasBounds").at("left").get_to(glyph.atlasBounds.x);
            j.at("atlasBounds").at("bottom").get_to(glyph.atlasBounds.y);
            j.at("atlasBounds").at("right").get_to(glyph.atlasBounds.z);
            j.at("atlasBounds").at("top").get_to(glyph.atlasBounds.w);
        }
    }

    MSDFFont::MSDFFont(const std::string &fontMetadataPath, const std::string &fontAtlasPath) {
        std::ifstream fontMetadataFile(fontMetadataPath);
        nlohmann::json fontMetadata = nlohmann::json::parse(fontMetadataFile);
        atlas_ = fontMetadata["atlas"].get<MSDFFont::Atlas>();
        name_ = fontMetadata["name"].get<std::string>();
        metrics_ = fontMetadata["metrics"].get<MSDFFont::Metrics>();
        const std::vector<MSDFFont::Glyph> &fontMetadataGlyphArray = fontMetadata["glyphs"].get<std::vector<MSDFFont::Glyph>>();
        std::transform(fontMetadataGlyphArray.begin(), fontMetadataGlyphArray.end(),
                       std::inserter(glyphs_, glyphs_.end()),
                       [](const MSDFFont::Glyph &glyph) { return std::pair<uint8_t, Glyph>(glyph.unicode, glyph); });

        texture_ = std::make_shared<render::Texture2D>(fontAtlasPath, true);
    }

    std::shared_ptr<render::Texture2D> MSDFFont::GetTexture() const {
        return texture_;
    }

    const std::map<uint32_t, MSDFFont::Glyph> &MSDFFont::GetGlyphs() const {
        return glyphs_;
    }

    const MSDFFont::Atlas &MSDFFont::GetAtlas() const {
        return atlas_;
    }

    const std::string &MSDFFont::GetName() const {
        return name_;
    }

    const MSDFFont::Metrics &MSDFFont::GetMetrics() const {
        return metrics_;
    }

    void to_json(nlohmann::json &nlohmann_json_j, const MSDFFont &font) {
        nlohmann_json_j["texture_"] = font.texture_->GetFilename();
        nlohmann_json_j["atlas_"] = font.atlas_;
        nlohmann_json_j["metrics_"] = font.metrics_;
        nlohmann_json_j["glyphs_"] = font.glyphs_;
    }

    void from_json(const nlohmann::json &nlohmann_json_j, MSDFFont &font) {
        std::string filename;
        nlohmann_json_j.at("texture_").get_to(filename);
        font.texture_ = std::make_shared<render::Texture2D>(filename, true);
        nlohmann_json_j.at("atlas_").get_to(font.atlas_);
        nlohmann_json_j.at("metrics_").get_to(font.metrics_);
        nlohmann_json_j.at("glyphs_").get_to(font.glyphs_);
    }

    glm::vec2 MSDFFont::CalculateTextDimensions(const std::string &text, const float scale) const {
        float x = 0.0f;
        float y = 0.0f;
        // TODO: is this copy really worth it to avoid std::map::at()?
        auto glyphs = glyphs_;
        for (const char &c: text) {
            const auto glyph = glyphs[c];

            if (glyph.planeBounds == glm::vec4(0.0f) || glyph.atlasBounds == glm::vec4(0.0f)) {
                x += glyph.advance * scale;
                continue;
            }

            float top = y + glyph.planeBounds.w * scale;

            x += glyph.advance * scale;
            // TODO: multiline text
            y = fmax(y, top);
        }
        return {x, y};
    }
}
