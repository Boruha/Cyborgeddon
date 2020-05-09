#include <Engine/SunlightEngine/SceneInterface/Resource/RModel.hpp>

#include <Engine/util/glad/glad.h>


#include <Engine/util/ErrorModelLoading.hpp>

extern "C" {
//	#define STB_IMAGE_IMPLEMENTATION // se define en otro .hpp, si no se encuentran los simbolos del archivo, descomentar esta linea
	#include <Engine/util/stb_image/stb_image.h>
}

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

constexpr bool strings_equal(const std::string_view a, const std::string_view b) {
	return a == b;
}

constexpr aiTextureType str2type(const std::string_view str) {
	if (strings_equal(str, DIFFUSE))
		return aiTextureType_DIFFUSE;
	else if (strings_equal(str, SPECULAR))
		return aiTextureType_SPECULAR;
	else if (strings_equal(str, NORMAL))
		return aiTextureType_NORMALS;
	else if (strings_equal(str, HEIGHT))
		return aiTextureType_HEIGHT;
	else
		return aiTextureType_NONE;
}

RModel::RModel(const std::string_view path) {
	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	ErrorModelLoading modelError(std::string(path) + "\n" + importer.GetErrorString());

	if (scene) {
		if (scene->mFlags & unsigned(AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
			modelError();

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	} else {
		modelError();
	}
}

RModel::~RModel() {
	for (const auto & t : modelTextures)
		glDeleteTextures(1, &t.ID);
}

void RModel::render(const glm::mat4 & m, Shader shader, bool visualShader) const {
	for (const auto & mesh : modelMeshes)
		mesh.render(m, shader, visualShader);
}

void RModel::processNode(aiNode * node, const aiScene * scene) {
	for (unsigned i = 0; i < node->mNumMeshes; ++i)
		processMesh(scene->mMeshes[node->mMeshes[i]], scene);
	for (unsigned i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], scene);
}

void RModel::processMesh(aiMesh * mesh, const aiScene * scene) {
	std::vector<Vertex>  vertices;
	std::vector<Index>   indices;
	std::vector<Texture> textures;

	processVertices(mesh, vertices);
	processIndices(mesh, indices);
	processTextures(mesh, scene, textures);

	modelMeshes.emplace_back(vertices, indices, textures);
}

void RModel::processVertices(aiMesh * mesh, std::vector<Vertex> & vertices) const {
	vertices.reserve(mesh->mNumVertices);

	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		glm::vec3 position  = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		glm::vec3 normal    = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		glm::vec2 texcoord  = glm::vec2();

		if (mesh->mTextureCoords[0])
			texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

		vertices.emplace_back(position, normal, texcoord);
	}
}

void RModel::processIndices(aiMesh * mesh, std::vector<Index> & indices) const {
	indices.reserve(mesh->mNumFaces);

	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
		for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
			indices.emplace_back(mesh->mFaces[i].mIndices[j]);
}


void RModel::processTextures(aiMesh * mesh, const aiScene * scene, std::vector<Texture> & textures) {
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuse, specular, normal, height;

		processTexturesByType(material, diffuse, DIFFUSE);
		processTexturesByType(material, specular, SPECULAR);
		processTexturesByType(material, normal, NORMAL);
		processTexturesByType(material, height, HEIGHT);

		textures.reserve(diffuse.size() + specular.size() + normal.size() + height.size());

		textures.insert(textures.end(), diffuse.begin(), diffuse.end());
		textures.insert(textures.end(), specular.begin(), specular.end());
		textures.insert(textures.end(), normal.begin(), normal.end());
		textures.insert(textures.end(), height.begin(), height.end());
	}
}

void RModel::processTexturesByType(aiMaterial * material, std::vector<Texture> & typeTextures, const std::string_view typeName) {
	const aiTextureType textureType = str2type(typeName);

	if (textureType == aiTextureType_NONE)
		error("Tipo de textura no soportada");

	for (unsigned i = 0; i < material->GetTextureCount(textureType); ++i) {
		aiString path;

		material->GetTexture(textureType, i, &path);

		if (!textureAlreadyStoredInModel(typeTextures, path.C_Str())) {
			Texture texture(loadTexture(path.C_Str()), typeName, path.C_Str());

			typeTextures.emplace_back(texture);
			modelTextures.emplace_back(texture);
		}
	}
}

bool RModel::textureAlreadyStoredInModel(std::vector<Texture> & textures, const std::string_view path) const {
	for (const auto & modelTexture : modelTextures) {
		if (strings_equal(modelTexture.path.data(), path)) {
			textures.emplace_back(modelTexture);
			return true;
		}
	}

	return false;
}

unsigned RModel::loadTexture(const std::string & path) const {
	std::string filename = directory + '/' + path;

	int width, height, formatColor;
	unsigned char * textureData = stbi_load(filename.c_str(), &width, &height, &formatColor, 0);

	if (!textureData)
		error("No se pudo cargar la textura " + filename);

	unsigned ID;
	glGenTextures(1, &ID);

	GLenum colorFormat { };

	switch (formatColor) {
		case 1  : colorFormat = GL_RED;  break;
		case 3  : colorFormat = GL_RGB;  break;
		case 4  : colorFormat = GL_RGBA; break;
		default : error("Formato de color no soportado en la textura " + path); break;
	}

	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);

	return ID;
}