#pragma once

#include <string_view>
#include <string>

struct IResource {
    virtual ~IResource() = default;

    [[nodiscard]] std::string_view getName() const {
        return name;
    };

    void setName(const std::string& _name) {
        name = _name;
    };

    virtual void loadResource(std::string_view) = 0;

    private:

        std::string name { };
};