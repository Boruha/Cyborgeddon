#include <Engine/EngineInterface/SceneInterface/ICameraNode.hpp>

#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNodeImpl.hpp>

#include <memory>

namespace irr::scene {
    struct ICameraSceneNode;
    struct ISceneManager;
}

struct IrrlichtCameraNode : ICameraNode {
    explicit IrrlichtCameraNode (
    		irr::scene::ISceneManager * = nullptr
    );

    ~IrrlichtCameraNode() override = default;

	explicit operator bool() const override;

	// removes node from scene
	void remove() override;

	// getters
	[[nodiscard]] const vec3 & getPosition() const override;
	[[nodiscard]] const vec3 & getRotation() const override;
	[[nodiscard]] const vec3 & getScale   () const override;

	// setters
	void setPosition(const vec3 &) override;
	void setRotation(const vec3 &) override;
	void setScale   (const vec3 &) override;

	// set texture
	void setTexture(std::string_view) const override;

	// sets if light should affect a node
	void affectedByLight(bool) const override;

    void setTarget(const vec3 & tar) override;

	[[nodiscard]] const vec3 &   getTarget()           const override;
	[[nodiscard]] const mat4x4 & getProjectionMatrix() 		 override;
	[[nodiscard]] const mat4x4 & getViewMatrix()       		 override;

    private :
        irr::scene::ICameraSceneNode * cameraNode   { nullptr };

        vec3 target {0};

        glm::mat4x4  projectionMatrix  {         };
        glm::mat4x4  viewMatrix        {         };

		std::unique_ptr<IrrlichtNodeImpl> p_impl { nullptr };
};