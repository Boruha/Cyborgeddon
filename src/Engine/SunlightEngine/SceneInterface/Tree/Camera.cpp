#include <Engine/SunlightEngine/SceneInterface/Tree/Camera.hpp>

#include <Engine/util/WindowParameters.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

void Camera::render(const glm::mat4& m) { };

void Camera::setPerspective(const float near, const float far, const float fov) {
	projection = glm::perspective(glm::radians(fov), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), near, far);
}

void Camera::setOrthogonal(const float left, const float right, const float bottom, const float top) {
	projection = glm::ortho(left, right, bottom, top);
}

void Camera::setTarget(const glm::vec3& tar) {
	target = tar;
}

void Camera::setViewMatrix(const glm::mat4 & m) {
	view = m;
}

const glm::vec3& Camera::getTarget() const {
	return target;
};

const glm::mat4& Camera::getViewMatrix() const {
	return view;
}

const glm::mat4& Camera::getProjectionMatrix() const {
	return projection;
}

glm::mat4 Camera::getViewProjectionMatrix() const {
	return projection * view;
}