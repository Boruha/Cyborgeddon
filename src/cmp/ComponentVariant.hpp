#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/AI.hpp>
#include <cmp/Physics.hpp>
#include <cmp/BulletData.hpp>
#include <cmp/CharacterData.hpp>
#include <vector>
#include <variant>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>

using std::vector;

using variantComponentVectorTypes = std::variant<
                                                   vector<AI>,
												   vector<std::unique_ptr<INode>>,
                                                   vector<Transformable>,
                                                   vector<Velocity>,
                                                   vector<BoundingBox>,
                                                   vector<Physics>,
                                                   vector<BulletData>,
                                                   vector<CharacterData>
                                                 >;