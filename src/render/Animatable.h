//
// Created by zach on 2023-05-07.
//

#ifndef GATHERSUN_RENDER_ANIMATABLE_H
#define GATHERSUN_RENDER_ANIMATABLE_H

#include "Core.h"

namespace gathersun::render {

    struct KeyFrame {
        glm::vec4 AtlasBounds = glm::vec4(0.0f);
        glm::vec4 PlaneBounds = glm::vec4(0.0f);

        friend void to_json(nlohmann::json &nlohmann_json_j, const KeyFrame &nlohmann_json_t) {
            nlohmann_json_j["atlasBounds"] = nlohmann::json({{"left",   nlohmann_json_t.AtlasBounds.x},
                                                             {"bottom", nlohmann_json_t.AtlasBounds.y},
                                                             {"right",  nlohmann_json_t.AtlasBounds.z},
                                                             {"top",    nlohmann_json_t.AtlasBounds.w}});
            nlohmann_json_j["planeBounds"] = nlohmann::json({{"left",   nlohmann_json_t.PlaneBounds.x},
                                                             {"bottom", nlohmann_json_t.PlaneBounds.y},
                                                             {"right",  nlohmann_json_t.PlaneBounds.z},
                                                             {"top",    nlohmann_json_t.PlaneBounds.w}});
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, KeyFrame &nlohmann_json_t) {
            nlohmann_json_j.at("atlasBounds").at("left").get_to(nlohmann_json_t.AtlasBounds.x);
            nlohmann_json_j.at("atlasBounds").at("bottom").get_to(nlohmann_json_t.AtlasBounds.y);
            nlohmann_json_j.at("atlasBounds").at("right").get_to(nlohmann_json_t.AtlasBounds.z);
            nlohmann_json_j.at("atlasBounds").at("top").get_to(nlohmann_json_t.AtlasBounds.w);
            nlohmann_json_j.at("planeBounds").at("left").get_to(nlohmann_json_t.PlaneBounds.x);
            nlohmann_json_j.at("planeBounds").at("bottom").get_to(nlohmann_json_t.PlaneBounds.y);
            nlohmann_json_j.at("planeBounds").at("right").get_to(nlohmann_json_t.PlaneBounds.z);
            nlohmann_json_j.at("planeBounds").at("top").get_to(nlohmann_json_t.PlaneBounds.w);
        }
    };

    struct Cycle {
        std::string Name = "Cycle"; // TODO: name this automatically based on cycle index by default
        uint8_t Index = 0;
        uint8_t StartingKeyFrame = 0;
        uint8_t NumKeyFrames = 0;

        friend void to_json(nlohmann::json &nlohmann_json_j, const Cycle &nlohmann_json_t) {
            nlohmann_json_j["name"] = nlohmann_json_t.Name;
            nlohmann_json_j["index"] = nlohmann_json_t.Index;
            nlohmann_json_j["startingKeyFrame"] = nlohmann_json_t.StartingKeyFrame;
            nlohmann_json_j["numKeyFrames"] = nlohmann_json_t.NumKeyFrames;
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, Cycle &nlohmann_json_t) {
            nlohmann_json_j.at("name").get_to(nlohmann_json_t.Name);
            nlohmann_json_j.at("index").get_to(nlohmann_json_t.Index);
            nlohmann_json_j.at("startingKeyFrame").get_to(nlohmann_json_t.StartingKeyFrame);
            nlohmann_json_j.at("numKeyFrames").get_to(nlohmann_json_t.NumKeyFrames);
        }
    };

    struct Animatable {
        uint8_t CurrentKeyFrame = 0;
        uint8_t NumKeyFrames = 1;
        uint8_t CurrentCycle = 0;
        uint8_t NumCycles = 1;
        float SecondsPerKeyFrame = 1.0f;
        double SubKeyFrame = 0.0f;
        std::vector<KeyFrame> KeyFrames;
        std::map<uint8_t, Cycle> Cycles;

        void SetCycle(const std::string &cycle) {
            uint8_t cycleIndex = std::find_if(Cycles.begin(), Cycles.end(), [cycle](const auto entry) {
                return entry.second.Name == cycle;
            })->second.Index;
            CurrentKeyFrame = Cycles[cycleIndex].StartingKeyFrame;
            CurrentCycle = cycleIndex;
        }

        void AdvanceKeyFrame(double dt) {
            SubKeyFrame += dt;
            if (SubKeyFrame >= SecondsPerKeyFrame) {
                SubKeyFrame -= SecondsPerKeyFrame;
                const auto currentCycle = Cycles[CurrentCycle];
                if (++CurrentKeyFrame >= currentCycle.StartingKeyFrame + currentCycle.NumKeyFrames) {
                    CurrentKeyFrame = currentCycle.StartingKeyFrame;
                }
            }
        }

        friend void to_json(nlohmann::json &nlohmann_json_j, const Animatable &nlohmann_json_t) {
            nlohmann_json_j["numKeyFrames"] = nlohmann_json_t.NumKeyFrames;
            nlohmann_json_j["numCycles"] = nlohmann_json_t.NumCycles;
            nlohmann_json_j["secondsPerKeyFrame"] = nlohmann_json_t.SecondsPerKeyFrame;
            nlohmann_json_j["keyFrames"] = nlohmann_json_t.KeyFrames;
            auto cyclesValueView = std::views::values(nlohmann_json_t.Cycles);
            std::vector<Cycle> cycles{cyclesValueView.begin(), cyclesValueView.end()};
            nlohmann_json_j["cycles"] = cycles;
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, Animatable &nlohmann_json_t) {
            Animatable nlohmann_json_default_obj;
            nlohmann_json_t.NumKeyFrames = nlohmann_json_j.value("numKeyFrames",
                                                                 nlohmann_json_default_obj.NumKeyFrames);
            nlohmann_json_t.NumCycles = nlohmann_json_j.value("numCycles", nlohmann_json_default_obj.NumCycles);
            nlohmann_json_t.SecondsPerKeyFrame = nlohmann_json_j.value("secondsPerKeyFrame",
                                                                       nlohmann_json_default_obj.SecondsPerKeyFrame);
            nlohmann_json_t.KeyFrames = nlohmann_json_j.value("keyFrames", nlohmann_json_default_obj.KeyFrames);
            std::vector<Cycle> cycles = nlohmann_json_j.value("cycles", std::vector<Cycle>{});
            std::for_each(cycles.begin(), cycles.end(), [&](auto cycleComponent) {
                nlohmann_json_t.Cycles.emplace(cycleComponent.Index, cycleComponent);
            });
        }
    };
}

#endif // GATHERSUN_RENDER_ANIMATABLE_H
