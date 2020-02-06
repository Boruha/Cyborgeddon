#pragma once

namespace Sun {
    struct Device;
}

struct IEngine;

struct TextureManager {
    explicit TextureManager(const Sun::Device& device) : device(device) {  }
    void loadTextures(/*const IEngine * engine*/) const;

    private :
        const Sun::Device& device;
};