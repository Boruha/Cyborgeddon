#pragma once

#include <string>
#include <glm/fwd.hpp>
#include <vector>

#include <Engine/SunlightEngine/SceneInterface/Resource/RMesh.hpp>

#include <Engine/SunlightEngine/SceneInterface/Resource/Vertex.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/Index.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/Texture.hpp>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

struct RModel : IResource {
	explicit RModel (std::string_view path);
	~RModel() override;

	void render(const glm::mat4 & m, Shader shader, bool visualShader) const;

	private :
		std::vector<RMesh> modelMeshes;
		inline static std::vector<Texture> modelTextures {};
		std::string directory;

		void processNode(aiNode * node, const aiScene * scene);
		void processMesh(aiMesh * mesh, const aiScene * scene);
		void processVertices(aiMesh * mesh, std::vector<Vertex> & vertices) const;
		void processIndices(aiMesh * mesh, std::vector<Index> & indices) const;
		void processTextures(aiMesh * mesh, const aiScene * scene, std::vector<Texture> & textures);
		void processTexturesByType(aiMaterial * material, std::vector<Texture> & typeTextures, std::string_view typeName);
		bool textureAlreadyStoredInModel(std::vector<Texture> &textures, std::string_view path) const;

		[[nodiscard]] unsigned loadTexture(const std::string& path) const;
};