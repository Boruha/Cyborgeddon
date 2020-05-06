#include <Engine/SunlightEngine/SceneInterface/Tree/Texture.hpp>

#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>
namespace Sun {

	Texture::Texture(ResourceManager *resManager, const std::string_view path) : resourceManager(*resManager) {
		if (!path.empty())
			texture = &resourceManager.getTexture(path);
	}

	void Texture::render() const {
		texture->render();
	}

	void Texture::setPosition(const unsigned int x, const unsigned int y) {
		texture->setPosition(x, y);
	}

	void Texture::setWidth(const unsigned int w) {
		texture->setWidth(w);
	}

	void Texture::setHeight(const unsigned int h) {
		texture->setHeight(h);
	}

	void Texture::setSize(const unsigned int w, const unsigned int h) {
		texture->setSize(w, h);
	}

};