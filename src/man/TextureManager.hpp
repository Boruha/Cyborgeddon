#pragma once

namespace Sun {
    struct Device;
}

struct Engine;

struct TextureManager {
    explicit TextureManager(const Sun::Device& device) : device(device) {  }
    void loadTextures(/*const Engine * engine*/) const;

    private :
        const Sun::Device& device;
};