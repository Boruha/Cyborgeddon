#pragma once

struct IResource {
    virtual ~IResource() = default;

    [[nodiscard]] const char * getName() const {
        return name;
    };
    void setName(char * const _name) {
        name = _name;
    };

    virtual void loadResource(const char *) = 0;

    private:

        char * name { nullptr };
};