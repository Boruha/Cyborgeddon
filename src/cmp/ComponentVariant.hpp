#pragma once

#include <cmp/Components.hpp>
#include <vector>
#include <variant>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>

using std::vector;
using Node_ptr = std::unique_ptr<INode>;

using variantComponentVectorTypes = std::variant<
                                                   vector<AI>,
                                                   vector<Transformable>,
                                                   vector<Velocity>,
                                                   vector<BoundingBox>,
                                                   vector<Physics>,
                                                   vector<BulletData>,
                                                   vector<CharacterData>,
                                                   vector<TriggerMovSphere>,
                                                   vector<TriggerStaticAABB>,
												   vector<RigidStaticAABB>
                                                 >;