//
// Created by zach on 2023-04-27.
//

#ifndef GATHERSUN_UTIL_H
#define GATHERSUN_UTIL_H

// TODO: Move this to something other than Util, I don't like naming things Util

#include "Core.h"

NLOHMANN_JSON_NAMESPACE_BEGIN
// TODO: should we just serialize glm types as an array?
template <>
struct adl_serializer<glm::vec4> {
    static void to_json(json& j, const glm::vec4& vec) {
        j = nlohmann::json{{"x", vec.x},
                           {"y", vec.y},
                           {"z", vec.z},
                           {"w", vec.w}};
    }

    static void from_json(const json& j, glm::vec4& vec) {
        j.at("x").get_to(vec.x);
        j.at("y").get_to(vec.y);
        j.at("z").get_to(vec.z);
        j.at("w").get_to(vec.w);
    }
};

template<>
struct adl_serializer<glm::vec3> {
    static void to_json(json& j, const glm::vec3& vec) {
        j = nlohmann::json{{"x", vec.x},
                           {"y", vec.y},
                           {"z", vec.z}};
    }

    static void from_json(const json& j, glm::vec3& vec) {
        j.at("x").get_to(vec.x);
        j.at("y").get_to(vec.y);
        j.at("z").get_to(vec.z);
    }
};

template<>
struct adl_serializer<glm::vec2> {
    static void to_json(json& j, const glm::vec2& vec) {
        j = nlohmann::json{{"x", vec.x},
                           {"y", vec.y}};
    }

    static void from_json(const json& j, glm::vec2& vec) {
        j.at("x").get_to(vec.x);
        j.at("y").get_to(vec.y);
    }
};

template<>
struct adl_serializer<glm::quat> {
    static void to_json(json& j, const glm::quat& quat) {
        j = nlohmann::json{{"x", quat.x},
                           {"y", quat.y},
                           {"z", quat.z},
                           {"w", quat.w}};
    }

    static void from_json(const json& j, glm::quat& quat) {
        j.at("x").get_to(quat.x);
        j.at("y").get_to(quat.y);
        j.at("z").get_to(quat.z);
        j.at("w").get_to(quat.w);
    }
};

NLOHMANN_JSON_NAMESPACE_END

#endif // GATHERSUN_UTIL_H
