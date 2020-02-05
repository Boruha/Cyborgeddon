#include <Engine/EngineInterface/SceneInterface/CameraNode.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNode.hpp>

namespace irr::scene {
    struct ICameraSceneNode;
    struct ISceneManager;
}

struct IrrlichtCameraNode final : CameraNode,  IrrlichtNode {
    explicit IrrlichtCameraNode (
            irr::scene::ISceneManager * = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr
    );

    ~IrrlichtCameraNode() final = default;

    void update() final;
    void update(float delta) final;

    [[nodiscard]] const glm::vec3 &   getTarget()           const final;
    [[nodiscard]] const glm::mat4x4 & getProjectionMatrix() const final;
    [[nodiscard]] const glm::mat4x4 & getViewMatrix()       const final;

    void setTarget(const glm::vec3 &) const final;
    void setTarget(float delta) const final;

    private :
        void preupdate(); // updates irrlicht matrix format to glm

        irr::scene::ICameraSceneNode * cameraNode   { nullptr };

        const glm::vec3 * target        { nullptr };
        glm::mat4x4  projectionMatrix   { };
        glm::mat4x4  viewMatrix         { };
};