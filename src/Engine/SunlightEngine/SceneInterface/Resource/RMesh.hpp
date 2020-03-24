#pragma once

#include <Engine/SunlightEngine/SceneInterface/Resource/IResource.hpp>

#include <Engine/SunlightEngine/SceneInterface/Resource/Vertex.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/Index.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/Texture.hpp>

#include <Engine/util/shaders/Shader.hpp>

#include <vector>

struct RMesh : IResource {
	explicit RMesh(std::vector<Vertex> _vertices, std::vector<Index> _indices, std::vector<Texture> _textures);
    ~RMesh() override;

	void render(const glm::mat4& m, Shader shader) const;

    private :

		unsigned VAO {0}, VBO {0}, EBO {0};

		std::vector<Vertex>  vertices;
		std::vector<Index>    indices;
		std::vector<Texture> textures;

		bool normal_tex      { false };
};