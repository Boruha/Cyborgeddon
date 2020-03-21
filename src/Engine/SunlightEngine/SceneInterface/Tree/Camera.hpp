#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct SceneManager;

struct Camera : IEntity {

    ~Camera() override = default;

    void render(const glm::mat4& m) override;

    void setPerspective(float near, float far, float fov = 45.f);

    void setOrthogonal(float left, float right, float bottom, float top);

    void setTarget(const glm::vec3& tar);

    void setViewMatrix(const glm::mat4& m);

    [[nodiscard]] const glm::vec3& getTarget() const;

    [[nodiscard]] const glm::mat4& getViewMatrix() const;
    [[nodiscard]] const glm::mat4& getProjectionMatrix() const;

	[[nodiscard]] glm::mat4 getViewProjectionMatrix() const;

    private :
		glm::mat4 projection { 1 };
        glm::mat4 view       { 1 };
        glm::vec3 target     { 0 };
};