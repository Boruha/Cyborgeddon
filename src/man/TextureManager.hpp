#pragma once

namespace Sun {
    struct Device;
}

struct TextureManager {
    explicit TextureManager(const Sun::Device& device) : device(device) {  }

    void loadTextures() const;

    private :
        const Sun::Device& device;
};