#pragma once

#include <string>

struct IEngine;

struct TextureManager {
	explicit TextureManager(const IEngine * const engine) : engine(*engine) {  };

    void loadTextures() const;
	void unloadTexture(const std::string& path) const;
	void unloadTextures() const;

	private :
		const IEngine& engine;
};