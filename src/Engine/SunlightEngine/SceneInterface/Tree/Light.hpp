#pragma once

#include "IEntity.hpp"

struct Light : IEntity {
    ~Light() override = default;

    void render(const mat4&) const override {  }

    void setValue(const vec3& newValue) { value = newValue; };
    [[nodiscard]] const vec3& getValue() const { return value; }

    private:

        glm::vec3 value { };
};