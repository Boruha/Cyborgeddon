#pragma once

namespace Sun {
    struct Device;
}

struct IEngine;

struct TextureManager {
	explicit TextureManager(const IEngine * const engine) : engine(engine) {  };

    void loadTextures() const;
    void unloadTextures() const;

	private :
		const IEngine * const engine { nullptr };
};