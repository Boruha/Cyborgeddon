#include <Engine/EngineInterface/SceneInterface/ICameraNode.hpp>

#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNodeImpl.hpp>

#include <memory>

namespace irr::scene {
    struct ICameraSceneNode;
    struct ISceneManager;
}

struct IrrlichtCameraNode : ICameraNode {
    explicit IrrlichtCameraNode (
            irr::scene::ISceneManager * = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr
    );

    ~IrrlichtCameraNode() override = default;

	explicit operator bool() const override;

	// removes node from scene
	void remove() override;

	// updates data immediately
	void update() override;
	// updates using interpolation based on parameter
	void update(float) override;

	// getters
	[[nodiscard]] const glm::vec3 & getPosition() const override;
	[[nodiscard]] const glm::vec3 & getRotation() const override;
	[[nodiscard]] const glm::vec3 & getScale   () const override;

	// setters
	void setPosition(const glm::vec3 &) const override;
	void setRotation(const glm::vec3 &) const override;
	void setScale   (const glm::vec3 &) const override;

	// setters using interpolation based on parameter
	void setPosition(float) const override;
	void setRotation(float) const override;
	void setScale   (float) const override;

	// set texture
	void setTexture(const char *) const override;

	// sets if light should affect a node
	void affectedByLight(bool) const override;

    void setTarget(const glm::vec3 & tar) const override;
    void setTarget(float delta) const override;

	[[nodiscard]] const glm::vec3 &   getTarget()           const override;
	[[nodiscard]] const glm::mat4x4 & getProjectionMatrix() const override;
	[[nodiscard]] const glm::mat4x4 & getViewMatrix()       const override;

    private :
        void preupdate(); // updates irrlicht matrix format to glm

        irr::scene::ICameraSceneNode * cameraNode   { nullptr };

        const glm::vec3 * const target { nullptr };
        glm::mat4x4  projectionMatrix  {         };
        glm::mat4x4  viewMatrix        {         };

		std::unique_ptr<IrrlichtNodeImpl> p_impl { nullptr };
};