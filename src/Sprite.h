//
// Created by zach on 2023-04-28.
//

#ifndef GATHERSUN_SPRITE_H
#define GATHERSUN_SPRITE_H

#include "Core.h"
#include "entity/Component.h"
#include "render/Texture.h"

namespace gathersun {

    struct SpriteComponent {
        // TODO: Standardize the naming of these internal json-only structs
        struct _Atlas {
            float Width;
            float Height;
            bool yOriginBottom;

            friend void to_json(nlohmann::json &nlohmann_json_j, const _Atlas &nlohmann_json_t) {
                nlohmann_json_j["width"] = nlohmann_json_t.Width;
                nlohmann_json_j["height"] = nlohmann_json_t.Height;
                nlohmann_json_j["yOrigin"] = nlohmann_json_t.yOriginBottom ? "bottom" : "top";
            }

            friend void from_json(const nlohmann::json &nlohmann_json_j, _Atlas &nlohmann_json_t) {
                nlohmann_json_j.at("width").get_to(nlohmann_json_t.Width);
                nlohmann_json_j.at("height").get_to(nlohmann_json_t.Height);
                nlohmann_json_t.yOriginBottom = nlohmann_json_j.at("yOrigin") == "bottom";
            }
        };

        struct AnimationComponent {

        };

        _Atlas Atlas;
        AnimationComponent Animation;

        friend void to_json(nlohmann::json &nlohmann_json_j, const SpriteComponent &nlohmann_json_t) {
            nlohmann_json_j["atlas"] = nlohmann_json_t.Atlas;
            nlohmann_json_j["animation"] = nlohmann_json_t.Animation;
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, SpriteComponent &nlohmann_json_t) {
            nlohmann_json_j.at("atlas").get_to(nlohmann_json_t.Atlas);
            nlohmann_json_j.at("animation").get_to(nlohmann_json_t.Animation);
        }

        glm::vec4 GetTexcoords() const {
            auto atlasBounds = Animation.KeyFrames[Animation.CurrentKeyFrame].AtlasBounds;

            if (!Atlas.yOriginBottom) {
                atlasBounds.y = Atlas.Height - atlasBounds.y;
                atlasBounds.w = Atlas.Height - atlasBounds.w;
            }

            return {
                    atlasBounds.x / Atlas.Width,
                    atlasBounds.y / Atlas.Height,
                    atlasBounds.z / Atlas.Width,
                    atlasBounds.w / Atlas.Height
            };
        }

        glm::vec4 GetSize() const {
            auto planeBounds = Animation.KeyFrames[Animation.CurrentKeyFrame].PlaneBounds;
            return {
                    planeBounds.x,
                    planeBounds.y,
                    planeBounds.z,
                    planeBounds.w
            };
        }
    };

    // TODO: Do we want to support multiple animated sprites in the same sheet? At the moment I'd rather just keep it to a single sprite.
    struct Sprite {
    public:
        Sprite() = default;
        Sprite(const std::string &spriteSheetFilename, const std::string &spriteSheetMetadataFilename)
                : texture_(spriteSheetFilename, true) {
            std::ifstream spriteSheetMetadataFile(spriteSheetMetadataFilename);
            nlohmann::json spriteSheetMetadata = nlohmann::json::parse(spriteSheetMetadataFile);
            spriteSheetMetadata.get_to(sprite_);
        }

        ~Sprite() = default;

        render::Texture2D &GetTexture() {
            return texture_;
        }

        glm::vec4 GetSize() const {
            return sprite_.GetSize();
        }

        glm::vec4 GetTexcoords() const {
            return sprite_.GetTexcoords();
        }

        void SetCycle(std::string cycle) {
            uint8_t cycleIndex = std::find_if(sprite_.Animation.Cycles.begin(), sprite_.Animation.Cycles.end(),
                                              [cycle](const auto entry) {
                                                  return entry.second.Name == cycle;
                                              })->second.Index;
            sprite_.Animation.CurrentKeyFrame = sprite_.Animation.Cycles[cycleIndex].StartingKeyFrame;
            sprite_.Animation.CurrentCycle = cycleIndex;
        }

        void AdvanceKeyFrame(double dt) {
            subkeyFrame_ += dt;
            if (subkeyFrame_ >= sprite_.Animation.SecondsPerKeyFrame) {
                subkeyFrame_ -= sprite_.Animation.SecondsPerKeyFrame;
                const auto currentCycle = sprite_.Animation.Cycles[sprite_.Animation.CurrentCycle];
                if (++sprite_.Animation.CurrentKeyFrame >= currentCycle.StartingKeyFrame + currentCycle.NumKeyFrames) {
                    sprite_.Animation.CurrentKeyFrame = currentCycle.StartingKeyFrame;
                }
            }
        }

    private:
        render::Texture2D texture_;
        SpriteComponent sprite_;
        float subkeyFrame_ = 0.0f;
    };
}

#endif // GATHERSUN_SPRITE_H
