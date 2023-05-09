//
// Created by zach on 2023-04-27.
//

#include "Serializer.h"

namespace gathersun {

    InputArchive::InputArchive(const std::string &jsonString) {
        root_ = nlohmann::json::parse(jsonString);
    }

    void InputArchive::operator()(entt::entity &entity) {
        entt::id_type id = current_[currentIndex_].get<entt::id_type>();
        entity = entt::entity(id);
        ++currentIndex_;
    }

    void InputArchive::operator()(std::underlying_type_t<entt::entity> &size) {
        next_();
        auto entitySize = current_[0].get<int>();
        ++currentIndex_;
        size = static_cast<std::underlying_type_t<entt::entity>>(entitySize);
    }

    void InputArchive::next_() {
        if (++rootIndex_ >= root_.size()) {
            // TODO: error handling
            return;
        }
        current_ = root_[rootIndex_];
        currentIndex_ = 0;
    }

    OutputArchive::OutputArchive() {
        startElement_();
    }

    OutputArchive::~OutputArchive() {
        endElement_();
    }

    void OutputArchive::operator()(entt::entity entity) {
        current_.push_back((entt::id_type) entity);
    }

    void OutputArchive::operator()(std::underlying_type_t<entt::entity> size) {
        endElement_();
        current_ = nlohmann::json::array();
        current_.push_back(size);
    }

    std::string OutputArchive::ToString() {
        return root_.dump(4);
    }

    void OutputArchive::startElement_() {
        root_ = nlohmann::json::array();
    }

    void OutputArchive::endElement_() {
        if (!current_.empty()) {
            root_.push_back(current_);
        }
    }
}
