#include <Engine/SunlightEngine/SceneInterface/Resource/RMesh.hpp>
#include <Engine/util/WindowParameters.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

RMesh::RMesh(std::vector<Vertex> _vertices, std::vector<Index> _indices, std::vector<Texture> _textures)
	: vertices(std::move(_vertices)), indices(std::move(_indices)), textures(std::move(_textures))
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(POSITION_LOCATION, POSITION_SIZE, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<void *>(POSITION_OFFSET));
	glVertexAttribPointer(NORMAL_LOCATION, NORMAL_SIZE, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<void *>(NORMAL_OFFSET));
	glVertexAttribPointer(TEXCOORD_LOCATION, TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<void *>(TEXCOORD_OFFSET));

	glEnableVertexAttribArray(POSITION_LOCATION);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glEnableVertexAttribArray(TEXCOORD_LOCATION);

	glBindVertexArray(0);	// desenlazamos el VAO
}

RMesh::~RMesh() = default;

void RMesh::render(const glm::mat4 & m, Shader shader) const {
	unsigned diffuse {0}, specular {0}, normal {0}, height {0};

	shader.enable();

	for (int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name = textures[i].type;

		if (std::string_view(name) == DIFFUSE)
			name += std::to_string(diffuse++);
		else if (std::string_view(name) == SPECULAR)
			name += std::to_string(specular++);
		else if (std::string_view(name) == NORMAL)
			name += std::to_string(normal++);
		else if (std::string_view(name) == HEIGHT)
			name += std::to_string(height++);

		shader.intUniform(name, i);

		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	shader.mat4Uniform("projection", glm::perspective(glm::radians(45.f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 1000.f));
	shader.mat4Uniform("view", glm::lookAt(glm::vec3(0, 10, 27.5), glm::vec3(0, 10, 27.5) + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));
	shader.mat4Uniform("model", m);

	shader.mat4Uniform("mvp", m);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void *>(0));

	glBindVertexArray(0);
}