cmake_minimum_required(VERSION 3.15)

project(Cyborgeddon)

set(CMAKE_CXX_STANDARD 17)

file (
    GLOB SOURCES

        "${PROJECT_SOURCE_DIR}/src/*.hpp"

        "${PROJECT_SOURCE_DIR}/src/cmp/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/ent/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/sys/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/man/*.hpp"

        "${PROJECT_SOURCE_DIR}/src/util/*.hpp"

        "${PROJECT_SOURCE_DIR}/src/Engine/EngineInterface/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/EngineInterface/SceneInterface/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/IrrlichtEngine/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/IrrlichtEngine/SceneInterface/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/SceneInterface/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/SceneInterface/Resource/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/SceneInterface/Tree/*.hpp"

        "${PROJECT_SOURCE_DIR}/src/Engine/util/glad/*.h"
        "${PROJECT_SOURCE_DIR}/src/Engine/util/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/util/shaders/*.hpp"

        "${PROJECT_SOURCE_DIR}/src/*.cpp"

        "${PROJECT_SOURCE_DIR}/src/cmp/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/ent/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/sys/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/man/*.cpp"

        "${PROJECT_SOURCE_DIR}/src/util/*.cpp"

        "${PROJECT_SOURCE_DIR}/src/Engine/IrrlichtEngine/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/IrrlichtEngine/SceneInterface/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/SceneInterface/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/SceneInterface/Resource/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/SunlightEngine/SceneInterface/Tree/*.cpp"

        "${PROJECT_SOURCE_DIR}/src/Engine/util/glad/*.c"
        "${PROJECT_SOURCE_DIR}/src/Engine/util/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Engine/util/shaders/*.cpp"

)

find_package ( OpenCV REQUIRED )

add_executable(cyborgeddon ${SOURCES} src/cmp/CircleBounding.hpp src/cmp/CircleBounding.cpp src/sys/CollisionSystem.hpp src/sys/CollisionSystem.cpp src/util/ColliderLoader.hpp src/cmp/TriangleOBB.hpp src/cmp/TriangleOBB.cpp src/Engine/SunlightEngine/SceneInterface/Resource/RTexture.hpp src/Engine/SunlightEngine/SceneInterface/Resource/RTexture.cpp src/Engine/SunlightEngine/SceneInterface/Tree/Texture.hpp src/Engine/SunlightEngine/SceneInterface/Tree/Texture.cpp src/Engine/EngineInterface/SceneInterface/ITexture.hpp src/sys/HUDintroSystem.hpp src/sys/HUDintroSystem.cpp src/cmp/TextureCmp.hpp src/cmp/TextureCmp.cpp src/cmp/MenuOption.hpp src/cmp/MenuOption.cpp src/sys/IntroInputSystem.hpp src/sys/IntroInputSystem.cpp src/sys/HUDingameSystem.hpp src/sys/HUDingameSystem.cpp)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src ${OpenCV_INCLUDE_DIRS})

target_link_libraries(cyborgeddon -lIrrlicht -lfmod -lfmodstudio -lglfw -lGLEW -lGL -lpthread -ldl -lassimp -lopencv_highgui -lopencv_core)