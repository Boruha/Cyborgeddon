#include <Engine/SunlightEngine/SceneInterface/Tree/TreeNode.hpp>

#include <glm/ext/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

TreeNode * TreeNode::addChildren(std::unique_ptr<TreeNode> child) {
    auto * child_ptr = child.get();
	children.emplace_back(std::move(child));
    child_ptr->parent = this;
    return child_ptr;
}

void TreeNode::removeChildren(TreeNode * child) {
	children.erase(std::find_if(children.begin(), children.end(), [child](const std::unique_ptr<TreeNode> & c) { return c.get() == child; }));
}

void TreeNode::setEntity(std::unique_ptr<IEntity> ent) {
    entity = std::move(ent);
}

void TreeNode::setParent(TreeNode * const p) {
    p->children.emplace_back(p);
    parent = p;
}

IEntity * TreeNode::getEntity() const {
    return entity.get();
}

TreeNode * TreeNode::getParent() const {
    return parent;
}

void TreeNode::render(const glm::mat4& m) {
	if (allowCalculateMatrix)
		transform = calculateMatrix();

    const glm::mat4 newMatrix = m * transform;

    if (entity)
        entity->render(sceneManager.getViewProjection() * newMatrix);

    if (!children.empty())
        for (const auto & child : children)
            child->render(newMatrix);
}

void TreeNode::setPosition(const vec3& trans) {
	allowCalculateMatrix = true;
    translation = vec3(trans.x, trans.y, -trans.z);
}

void TreeNode::setRotation(const vec3& rot) {
	allowCalculateMatrix = true;
    rotation = vec3(rot.x, rot.y, rot.z);
}

void TreeNode::setScale(const vec3& sca) {
	allowCalculateMatrix = true;
	scale = sca;
}

void TreeNode::toTranslate(const vec3& v) {
	allowCalculateMatrix = true;
    translation += v;
}

void TreeNode::toRotate(const vec3& v) {
	allowCalculateMatrix = true;
    rotation += v;
}

void TreeNode::toScale(const vec3& v) {
	allowCalculateMatrix = true;
    scale += v;
}

const vec3& TreeNode::getPosition() const {
    return translation;
}

const vec3& TreeNode::getRotation() const {
    return rotation;
}

const vec3& TreeNode::getScale() const {
    return scale;
}

void TreeNode::setTransform(const glm::mat4& m) {
	allowCalculateMatrix = true;
    transform = m;
}

const glm::mat4& TreeNode::getTransform() const {
    return transform;
}

glm::mat4 TreeNode::calculateMatrix() {
	allowCalculateMatrix = false;
    return 		glm::translate(mat4(1), translation)
    		* 	glm::rotate(mat4(1), glm::radians(rotation.x), vec3(1,0,0))
		 	* 	glm::rotate(mat4(1), glm::radians(rotation.y), vec3(0,1,0))
		 	* 	glm::rotate(mat4(1), glm::radians(rotation.z), vec3(0,0,1))
		  	* 	glm::scale(mat4(1), scale);
}

void TreeNode::remove() {
	if (parent)
		parent->removeChildren(this);
}
