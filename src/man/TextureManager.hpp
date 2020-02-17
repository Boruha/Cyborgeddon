#pragma once

struct IEngine;

struct TextureManager {
	explicit TextureManager(const IEngine * const engine) : engine(engine) {  };

    void loadTextures() const;
    void unloadTexture(const char * path) const;
    void unloadTextures() const;

	private :
		const IEngine * const engine { nullptr };
};