#pragma once

#include <cmp/Component.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/AI.hpp>
#include <cmp/Physics.hpp>
#include <cmp/BulletData.hpp>
#include <cmp/CharacterData.hpp>
#include <vector>
#include <variant>

using std::vector;

using variantComponentVectorTypes = std::variant<vector<AI>, vector<Node_ptr>, vector<Transformable>, vector<Velocity>, vector<BoundingBox>, vector<Physics>, vector<BulletData>, vector<CharacterData>>;
using variantComponentTypes = std::variant<AI, Node_ptr, Transformable, Velocity, BoundingBox, Physics, BulletData, CharacterData>;
// TODO: comprobar const-correctness de todos los componentes y hacer los cambios pertinentes