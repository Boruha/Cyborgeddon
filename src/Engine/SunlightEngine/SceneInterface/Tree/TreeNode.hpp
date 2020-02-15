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

    void render(const mat4x4& m) const;

    void toTranslate(const vec3& v);
    void toRotate(const vec3& v);
    void toScale(const vec3& v);

    void addChildren(TreeNode * child);
    void removeChildren(TreeNode * child);

    void setEntity(IEntity * ent);
    void setParent(TreeNode * parent);

    void setTranslation(const vec3& trans);
    void setRotation(const vec3& rot);
    void setScale(const vec3& sca);

    void setTransform(const mat4x4& m);

    [[nodiscard]] IEntity * getEntity() const;
    [[nodiscard]] TreeNode * getParent() const;


    [[nodiscard]] const vec3& getTranslation() const;
    [[nodiscard]] const vec3& getRotation() const;
    [[nodiscard]] const vec3& getScale() const;

    [[nodiscard]] const mat4x4& getTransform() const;

    private :
        [[nodiscard]] mat4x4 calculateMatrix() const;

        IEntity * entity { nullptr };

        TreeNode * parent { nullptr };

        vector<TreeNode *> children { };

        vec3 translation { };
        vec3 rotation { };
        vec3 scale { };

        mat4x4 transform { };
};