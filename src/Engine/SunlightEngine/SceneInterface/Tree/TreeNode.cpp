#include <Engine/SunlightEngine/SceneInterface/Tree/TreeNode.hpp>
#include <iostream>

void TreeNode::addChildren(TreeNode * const child) {
    children.emplace_back(child);
    child->setParent(this);
}

void TreeNode::removeChildren(TreeNode * const child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void TreeNode::setEntity(IEntity * const ent) {
    entity = ent;
}

void TreeNode::setParent(TreeNode * const p) {
    parent = p;
}

IEntity * TreeNode::getEntity() const {
    return entity;
}

TreeNode * TreeNode::getParent() const {
    return parent;
}

void TreeNode::render(const mat4& m) const {

	std::cout << "Soy el nodo " << ID;
	if (parent)
		std::cout << ", y mi padre es " << parent->ID;
	std::cout << "\n";

    const mat4 newMatrix = m * calculateMatrix();

    if (entity)
        entity->render(newMatrix);

    if (!children.empty())
        for (const auto& child : children)
            child->render(newMatrix);
}

void TreeNode::setTranslation(const vec3& trans) {
    translation = trans;
}

void TreeNode::setRotation(const vec3& rot) {
    rotation = rot;
}

void TreeNode::setScale(const vec3& sca) {
    scale = sca;
}

void TreeNode::toTranslate(const vec3& v) {
    translation += v;
}

void TreeNode::toRotate(const vec3& v) {
    rotation += v;
}

void TreeNode::toScale(const vec3& v) {
    scale += v;
}

const vec3& TreeNode::getTranslation() const {
    return translation;
}

const vec3& TreeNode::getRotation() const {
    return rotation;
}

const vec3& TreeNode::getScale() const {
    return scale;
}

void TreeNode::setTransform(const mat4& m) {
    transform = m;
}

const mat4& TreeNode::getTransform() const {
    return transform;
}

mat4 TreeNode::calculateMatrix() const {
    return 	  glm::scale(transform, scale)
    		* glm::rotate(transform, glm::radians(rotation.x), vec3(1,0,0))
    		* glm::rotate(transform, glm::radians(rotation.y), vec3(0,1,0))
		  	* glm::rotate(transform, glm::radians(rotation.z), vec3(0,0,1))
		  	* glm::translate(transform, translation);
}