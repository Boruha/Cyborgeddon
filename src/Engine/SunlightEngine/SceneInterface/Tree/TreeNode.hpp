#pragma once

#include <glm/ext/matrix_transform.hpp>

#include <vector>
#include <memory>

#include <Engine/EngineInterface/SceneInterface/INode.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>
#include "Camera.hpp"

using glm::vec2;
using glm::vec3;
using glm::mat4;

struct SceneManager;
struct Shader;

struct TreeNode final : INode {

	explicit TreeNode(SceneManager& _sceneManager) : sceneManager(_sceneManager) { }

	explicit operator bool() const override { return true; };

    void render(const mat4& m, Shader shader);

	void remove() override;

    void toTranslate(const vec3& v);
    void toRotate(const vec3& v);
    void toScale(const vec3& v);

    TreeNode * addChildren(std::unique_ptr<TreeNode> child);
    void removeChildren(TreeNode * child);

    void setEntity(std::unique_ptr<IEntity> ent);
    void setParent(TreeNode * parent);

    void setPosition(const vec3& pos) override;
    void setRotation(const vec3& rot) override;
    void setScale(const vec3& sca) override;

    void setTransform(const mat4& m);

    [[nodiscard]] IEntity * getEntity() const;
    [[nodiscard]] TreeNode * getParent() const;


    [[nodiscard]] const vec3& getPosition() const override;
    [[nodiscard]] const vec3& getRotation() const override;
    [[nodiscard]] const vec3& getScale() const override;

    [[nodiscard]] const mat4& getTransform() const;


	[[nodiscard]] const vec3& getTarget() const override { return vec3(); }

	// setters
	void setTarget(const vec3& tar) override;

	// set texture
	void setTexture(std::string_view) const override {};

	// sets if light should affect a node
	void affectedByLight(bool) const override {/*TODO: SERA UN UNIFORM*/};

	void calculateTransform() { transform = calculateMatrix(); }

	private :
		[[nodiscard]] mat4 calculateMatrix();

        std::unique_ptr<IEntity> entity { nullptr };

        TreeNode * parent { nullptr };

        std::vector<std::unique_ptr<TreeNode>> children { };

		vec3 translation { 0 };
		vec3 rotation    { 0 };
		vec3 scale       { 1 };

		mat4 transform   { 1 };
        mat4 modelView   { 1 };
        mat4 m_normal    { 1 };

		bool allowCalculateMatrix { true };

		SceneManager& sceneManager;

		const std::size_t ID { nextID++ };

		inline static std::size_t nextID { 0 };
};