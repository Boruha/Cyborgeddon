#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <vector>
#include <algorithm>

#include <src/Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

using std::vector;
using glm::vec3;
using glm::mat4x4;

struct TreeNode {

    void addChildren(TreeNode * const child) {
        children.emplace_back(child);
    };

    void removeChildren(TreeNode * const child) {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    };

    void setEntity(IEntity * const ent) {
        entity = ent;
    };

    [[nodiscard]] IEntity * getEntity() const {
        return entity;
    };

    [[nodiscard]] TreeNode * getParent() const {
        return parent;
    };

    void render(const mat4x4& m) const {
        const mat4x4 newMatrix = m * calculateMatrix();

        if (entity)
            entity->render(newMatrix);

        if (!children.empty())
            for (const auto& child : children)
                child->render(newMatrix);
    }

    void setTranslation(const vec3& trans) {
        translation = trans;
    }

    void setRotation(const vec3& rot) {
        rotation = rot;
    }

    void setScale(const vec3& sca) {
        scale = sca;
    }

    void toTranslate(const vec3& v) {
        translation += v;
    }

    void toRotate(const vec3& v) {
        rotation += v;
    }

    void toScale(const vec3& v) {
        scale += v;
    }

    [[nodiscard]] const vec3& getTranslation() const {
        return translation;
    }

    [[nodiscard]] const vec3& getRotation() const {
        return rotation;
    }

    [[nodiscard]] const vec3& getScale() const {
        return scale;
    }

    void setTransform(const mat4x4& m) {
        transform = m;
    }

    [[nodiscard]] const mat4x4& getTransform() const {
        return transform;
    }

    private :
        IEntity * entity { nullptr };

        TreeNode * parent { nullptr };

        vector<TreeNode *> children { };

        vec3 translation { };
        vec3 rotation { };
        vec3 scale { };

        mat4x4 transform { };

        [[nodiscard]] mat4x4 calculateMatrix() const {
            return glm::scale(transform, scale) * glm::rotate(transform, 0.f, rotation) * glm::translate(transform, translation);
        }
};