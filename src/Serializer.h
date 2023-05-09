//
// Created by zach on 2023-04-27.
//

#ifndef GATHERSUN_SERIALIZER_H
#define GATHERSUN_SERIALIZER_H

#include "Core.h"
#include <nlohmann/json.hpp>
#include <fstream>

// TODO: Add macro/tag/etc to flag a component as serializable, possibly with the save file name? (for multiple .json files)

namespace gathersun {

    class InputArchive {
    public:
        explicit InputArchive(const std::string &jsonString);

        ~InputArchive() = default;

        void operator()(entt::entity &entity);

        void operator()(std::underlying_type_t<entt::entity> &size);

        template<typename T>
        void operator()(entt::entity &entity, T &component);

    private:
        nlohmann::json root_;
        nlohmann::json current_;
        int rootIndex_ = -1;
        int currentIndex_ = 0;

        void next_();
    };

    template<typename T>
    void InputArchive::operator()(entt::entity &entity, T &component) {
        nlohmann::json componentJson = current_[currentIndex_ * 2];

        auto componentData = componentJson.get<T>();
        component = componentData;

        entt::id_type entityId = current_[currentIndex_ * 2 - 1];
        entity = entt::entity(entityId);
        ++currentIndex_;
    }

    class OutputArchive {
    public:
        OutputArchive();

        ~OutputArchive();

        void operator()(entt::entity entity);

        void operator()(std::underlying_type_t<entt::entity> size);

        template<typename T>
        void operator()(entt::entity entity, const T &component);

        std::string ToString();

    private:
        nlohmann::json root_;
        nlohmann::json current_;

        void startElement_();

        void endElement_();
    };

    template<typename T>
    void OutputArchive::operator()(entt::entity entity, const T &component) {
        this->operator()(entity);
        nlohmann::json componentJson = component;
        current_.push_back(componentJson);
    }

    template<typename... ComponentTypes>
    class Serializer {
    public:
        Serializer(std::string filename);

        ~Serializer() = default;

        void Serialize(const entt::registry &registry);

        void Deserialize(entt::registry &registry);

    private:
        std::string filename_;
    };

    template<typename... ComponentTypes>
    Serializer<ComponentTypes...>::Serializer(std::string filename) : filename_(std::move(filename)) {

    }

    template<typename... ComponentTypes>
    void Serializer<ComponentTypes...>::Serialize(const entt::registry &registry) {
        std::string json = "{}";
        {
            OutputArchive archive;
            entt::snapshot{registry}.entities(archive).component<ComponentTypes...>(archive);
            json = archive.ToString();
        }

        std::ofstream file(filename_, std::ios::trunc);
        file << json;
        file.close();
    }

    template<typename... ComponentTypes>
    void Serializer<ComponentTypes...>::Deserialize(entt::registry &registry) {
        std::ifstream file(filename_);
        std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        InputArchive archive(jsonString);
        entt::snapshot_loader{registry}.entities(archive).component<ComponentTypes...>(archive);
    }
}

#endif // GATHERSUN_SERIALIZER_H
