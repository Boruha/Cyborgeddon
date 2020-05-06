#include <sys/AI_System.hpp>

#include <Engine/util/MathIntersection.hpp>
#include <util/SystemConstants.hpp>
#include <util/SoundPaths.hpp>
#include <util/Pathfinding.hpp>

#include <chrono>
#include <thread>
#include <algorithm>

using namespace std::chrono;

constexpr std::chrono::duration<double> LOD_PA_PERIOD (  5us );
constexpr std::chrono::duration<double> PATROL_PERIOD ( 10us );
constexpr std::chrono::duration<double> PURSUE_PERIOD ( 20us );
constexpr std::chrono::duration<double> ATTACK_PERIOD ( 10us );

constexpr float movement_per_frame { 3.0 }; //BASE ON MOVEMENT CONST VARIBLES;

/*  GENERAL BEHAVIOURS  */
    struct SeekBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            vel.direction = vec3(ai.target_position.x - phy.position.x, 0, ai.target_position.z - phy.position.z);
            phy.velocity  = normalize(vel.direction) * vel.currentSpeed * deltaTime;

            return true;
        }
    };

    struct AlignBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            phy.rotation.y = nearestAngle(phy.rotation.y, getRotationYfromXZ(phy.position - ai.target_position));

            return true;
        }
    };

    struct ArriveBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            const float distance2 = length2 ({ phy.position.x - ai.target_position.x, phy.position.z - ai.target_position.z });

            return greater_e(distance2, ARRIVE_MIN_DISTANCE2);
        }
    };

    struct jumpingBehaviour : BehaviourNode //Need previous condition which set jumping to true
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            auto & enemy    = context->getEntityByID(ai.getEntityID());
			auto & jump     = *enemy.getComponent<Jump>();
            
            if(jump.jumping) 
            {
                ai.target_position = jump.jumpTargetLocation;
                return true;
            }
            return false;
        }
    };

    struct jumpUpdateBehaviour : BehaviourNode 
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            auto & enemy    = context->getEntityByID(ai.getEntityID());
			auto & jump     = *enemy.getComponent<Jump>();

            vel.direction = vec3(ai.target_position.x - phy.position.x, 0, ai.target_position.z - phy.position.z);
            phy.velocity  = normalize(vel.direction) * vel.currentSpeed * deltaTime;
            
            jump.jumpTimer -= deltaTime * 2;
            phy.velocity.y = jump.jumpTimer * (vel.currentSpeed * 2) * deltaTime;  
            
            if(phy.position.y < (phy.scale.y / 2))
            {
                jump.jumpTimer = 1.f;
                phy.position.y = phy.scale.y / 2;
                phy.velocity.y = 0;
                jump.jumping = false;
            }
            return true;
        }
    };

    struct obstacleAvoidanceBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            auto & enemy    = context->getEntityByID(ai.getEntityID());
            auto & trSphere = *enemy.getComponent<TriggerMovSphere>();
            
            if(!checkObstacles(phy.position, player_pos, trSphere.radius, context))
                return true;

            return false;
        }
    };
/*  GENERAL BEHAVIOURS  */

/*  LOD PA  BEHAVIORS   */
    struct NextTeletransportBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {            
            //LOOK FORWARD
            unsigned future_index = (ai.patrol_index + 1) % ai.max_index;
            vec3     future_pos   = ai.patrol_position[future_index];
                        
            //CHECK FRAME INTERVAL
            float distance2 = length2({ future_pos.x - ai.target_position.x, future_pos.z - ai.target_position.z });
            distance2       = std::sqrt(distance2);
            
            if( (ai.frameCounter - ai.lastFrameEjecuted) > movement_per_frame * distance2 )
            {
                ai.patrol_index      = future_index;
                phy.position         = future_pos;
                ai.lastFrameEjecuted = ai.frameCounter;
            }            

            return true;
        }
    };

    struct LODStateBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
            
            if(greater_e(distance2, LOD_PA_MIN_DISTANCE2))
            {
                ai.target_position = ai.patrol_position[ai.patrol_index];
                ai.frequecy_state = LOD_PA_STATE;
                return true;
            }

            return false;
        }
    };
/*  LOD PA  BEHAVIORS   */

/*  PATROL BEHAVIOURS  */
    struct NextPatrolBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            ai.patrol_index    = (ai.patrol_index + 1) % ai.max_index;
            vec3 myPos = ai.target_position;
            ai.target_position = ai.patrol_position[ai.patrol_index];
            
            if(ai.getEntityID() == 60)
            {
                float distance2 = length2({ myPos.x - ai.target_position.x, myPos.z - ai.target_position.z });
                distance2 = std::sqrt(distance2);

                std::cout << "LAST FRAME INTERVAL: " << ai.frameCounter - ai.lastFrameEjecuted << "\n\n";
                ai.lastFrameEjecuted = ai.frameCounter;
                std::cout << "DISTANCIA : " << distance2 << ",  FRAMES QUE ESPERARÉ: " << distance2 * movement_per_frame << "\n";
            }

            return true;
        }
    };

    struct PatrolStateBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
            
            if(greater_e(distance2, PATROL_MIN_DISTANCE2) ||
              (greater_e(distance2, VIEW_MIN_DISTANCE2)   && !checkFacing(phy, context)) )
            {
                ai.target_position = ai.patrol_position[ai.patrol_index];
                ai.frequecy_state = PATROL_STATE;
                return true;
            }

            return false;
        }
    };
/*  PATROL BEHAVIOURS  */

/*  PURSE BEHAVIOURS  */
    struct PursueStateBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            auto & enemy    = context->getEntityByID(ai.getEntityID());
			auto * jump     = enemy.getComponent<Jump>();

            ai.frequecy_state = PURSUE_STATE;
            ai.target_position = player_pos;

            const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });            
            
            if(jump)
                return greater_e(distance2, PURSUE_MIN_DISTANCE2) && !jump->jumping;
            else
                return greater_e(distance2, PURSUE_MIN_DISTANCE2);
        }

    };

    struct HaveRouteBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            if(ai.path_index > -1)
            {
                const std::vector<MapNode>& ref_graph = context->getGraph();
                ai.target_position = ref_graph[ai.path_node].coord;

                return true;
            }
            return false;
        }
    };
    //Se deberia comprobar que se genere todo(?)
    struct CreateRouteBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            const std::vector<MapNode>& ref_graph = context->getGraph();

            int final_path = nearestNode(player_pos, ref_graph);       //index -> mapnode + cercano a player
            int ini_path   = nearestNode(phy.position, ref_graph);     //index -> mapnode + cercano a enemy
            ai.path_node   = ini_path;
            ai.path_index  = 0;
            //guardamos el path generado, usamos el ID para identificarlo despues.
            context->setPath(ai.getEntityID(), calculePath(ini_path, final_path, ref_graph));
            ai.target_position = ref_graph[ai.path_node].coord;
            
            return true;
        }
    };

    struct NextPursePointBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            const std::vector<MapNode>& ref_graph = context->getGraph();
            std::vector<int>& ref_path = context->getPath(ai.getEntityID());

            if(unsigned(++ai.path_index) < ref_path.size())
            {
                ai.path_node  = ref_path[ai.path_index];
                ai.target_position = ref_graph[ai.path_node].coord;
                return true;
            }
            return false;
        }
    };

    struct DeletePurseBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            if(ai.path_index != -1)
            {
                ai.path_index = -1;
                ai.path_node  = -1;
                context->deletePath(ai.getEntityID());
            }
            return false;
        }
    };

    struct PathConditionBehaviour : BehaviourNode
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });

            return !greater_e(distance2, VIEW_MIN_DISTANCE2);
        }
    };

/*  PURSE BEHAVIOURS  */

/*  ATTACK BEVAHOIUR  */
    struct AttackStateBehaviour : BehaviourNode //innecesario? ad future puede tener un parametro no distancia. ai.state cambiarlo a una condition? 
    {
        bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
        {
            ai.frequecy_state = ATTACK_STATE;
            ai.target_position = player_pos;
            phy.velocity = glm::vec3(0);
            return true;
        }
    };

    /*  BASIC ATTACK  */
        struct BasicAttackBehaviour : BehaviourNode 
        {
            bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
            {
                auto & enemy    = context->getEntityByID(ai.getEntityID());
			    auto & data     = *enemy.getComponent<CharacterData>();
                
                if(!greater_e(data.currentAttackingCooldown, 0.f)) 
                {
                    data.attacking = true;
                    data.currentAttackingCooldown = data.attackingCooldown;
                    //damageMessages.emplace_back(data.attackDamage);
                    soundMessages.emplace_back(ATTACK_ENEMY_ASSEMBLY);
                    return true;
                }

                return false;
            }
        };

        struct RangeBasicAttackBehaviour : BehaviourNode 
        {
            bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
            {
                auto & enemy    = context->getEntityByID(ai.getEntityID());
			    auto & data     = *enemy.getComponent<CharacterData>();
                
                const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });

                return greater_e(distance2, data.attackRange);
            }
        };
    /*  BASIC ATTACK  */

    /*  DEMON ATTACK  */
        struct DemonBehaviour : BehaviourNode 
        {
            bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
            {
                auto & enemy    = context->getEntityByID(ai.getEntityID());
			    auto & data     = *enemy.getComponent<CharacterData>();
                
	            return data.mode == DEMON;
            }
        };

        struct jumpDmgBehaviour : BehaviourNode 
        {
            bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
            {
                auto & enemy    = context->getEntityByID(ai.getEntityID());
			    auto & jump     = *enemy.getComponent<Jump>();
                
                if(!jump.jumping)
                {
                    const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
                    
                    if( !greater_e(distance2, JUMP_AREA_DMG2) )
                        std::cout << "TORNADITO QUE TE METO EN MOVIMIENTO\n";
                }
                return true;
            }
        };

        struct tryJumpBehaviour : BehaviourNode 
        {
            bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
            {
                auto & enemy    = context->getEntityByID(ai.getEntityID());
			    auto & jump     = *enemy.getComponent<Jump>();
                
                const float distance2 = length2({ phy.position.x - ai.target_position.x, phy.position.z - ai.target_position.z });

                if(!greater_e(distance2, DIST_ATTACK_RANGE2) && greater_e(distance2, MELEE_ATTACK_RANGE2) 
                && !greater_e(jump.currentJumpCooldown, 0))
                {
                    jump.jumpTargetLocation = player_pos;
                    jump.jumping = true;
                    jump.currentJumpCooldown = jump.jumpCooldown;
                    return true;
                }   
                return false;
            }
        };
    /*  DEMON ATTACK  */

/*  ATTACK BEVAHOIUR  */


/* FUNCTIONS */
void AI_System::init() 
{
    /*-- LOD PA  --*/
        std::unique_ptr<Sequence> lodPatrolState = std::make_unique<Sequence>();
        lodPatrolState->childs.emplace_back(std::make_unique<LODStateBehaviour>());
        lodPatrolState->childs.emplace_back(std::make_unique<NextTeletransportBehaviour>());

    /*-- LOD PA  --*/

    /*-- PATROL  --*/     
        /* PATROL UPDATE */
        std::unique_ptr<Selector> patrolPoint = std::make_unique<Selector>();
        patrolPoint->childs.emplace_back(std::make_unique<ArriveBehaviour>());
        patrolPoint->childs.emplace_back(std::make_unique<NextPatrolBehaviour>());

        /* PHY UPDATE */
        std::unique_ptr<Sequence> phyUpdate = std::make_unique<Sequence>();
        phyUpdate->childs.emplace_back(std::make_unique<AlignBehaviour>());
        phyUpdate->childs.emplace_back(std::make_unique<SeekBehaviour>());

        /* PATROL STATE */
        std::unique_ptr<Sequence> patrolState = std::make_unique<Sequence>();
        patrolState->childs.emplace_back(std::make_unique<PatrolStateBehaviour>());
        patrolState->childs.emplace_back(std::move(patrolPoint));
        patrolState->childs.emplace_back(std::move(phyUpdate));

    /*-- PATROL  --*/

    /*-- PURSUE  --*/     
        /* UPDATE PURSUE */
        std::unique_ptr<Selector> pursuePoint = std::make_unique<Selector>();
        pursuePoint->childs.emplace_back(std::make_unique<ArriveBehaviour>());
        pursuePoint->childs.emplace_back(std::make_unique<NextPursePointBehaviour>());
        pursuePoint->childs.emplace_back(std::make_unique<DeletePurseBehaviour>());

        /* GET PURSUE */
        std::unique_ptr<Sequence> getPursue = std::make_unique<Sequence>();
        getPursue->childs.emplace_back(std::make_unique<HaveRouteBehaviour>());
        getPursue->childs.emplace_back(std::move(pursuePoint));

        /* SET/GET PURSE */
        std::unique_ptr<Selector> setGetPursue  = std::make_unique<Selector>();
        setGetPursue->childs.push_back(std::move(getPursue));
        setGetPursue->childs.emplace_back(std::make_unique<CreateRouteBehaviour>());

        /* PATH BRANCH */
        std::unique_ptr<Sequence> pathBranch  = std::make_unique<Sequence>();
        pathBranch->childs.emplace_back(std::make_unique<PathConditionBehaviour>());
        pathBranch->childs.emplace_back(std::move(setGetPursue));

        /* WAY SELECTOR */
        std::unique_ptr<Selector> waySelector  = std::make_unique<Selector>();
        waySelector->childs.emplace_back(std::make_unique<obstacleAvoidanceBehaviour>());
        waySelector->childs.emplace_back(std::move(pathBranch));

        /* PHY UPDATE */
        phyUpdate = std::make_unique<Sequence>();
        phyUpdate->childs.emplace_back(std::make_unique<AlignBehaviour>());
        phyUpdate->childs.emplace_back(std::make_unique<SeekBehaviour>());

        /* PURSUE STATE */
        std::unique_ptr<Sequence> pursueState = std::make_unique<Sequence>();
        pursueState->childs.emplace_back(std::make_unique<PursueStateBehaviour>());
        pursueState->childs.emplace_back(std::move(waySelector));
        pursueState->childs.emplace_back(std::move(phyUpdate));
    /*-- PURSUE  --*/

    /*-- ATTACK  --*/ 
        /* BASIC ATTACK */
            /* PHY UPDATE */
            phyUpdate = std::make_unique<Sequence>();
            phyUpdate->childs.emplace_back(std::make_unique<RangeBasicAttackBehaviour>());
            phyUpdate->childs.emplace_back(std::make_unique<SeekBehaviour>());

            /* ACTION SELECTOR*/
            std::unique_ptr<Selector> actionSelector = std::make_unique<Selector>();
            actionSelector->childs.push_back(std::move(phyUpdate));
            actionSelector->childs.emplace_back(std::make_unique<BasicAttackBehaviour>());  

            /* ATTACK */
            std::unique_ptr<Sequence> basicAttack = std::make_unique<Sequence>();
            basicAttack->childs.emplace_back(std::make_unique<AlignBehaviour>());
            basicAttack->childs.push_back(std::move(actionSelector));
        /* BASIC ATTACK */

        /* DEMON BRANCH */
            /* UPDATE JUMP */
            std::unique_ptr<Sequence> jumpUpdate = std::make_unique<Sequence>();
            jumpUpdate->childs.emplace_back(std::make_unique<jumpingBehaviour>());
            jumpUpdate->childs.emplace_back(std::make_unique<jumpUpdateBehaviour>());
            jumpUpdate->childs.emplace_back(std::make_unique<jumpDmgBehaviour>());

            /* START JUMP */
            std::unique_ptr<Sequence> jumpStart = std::make_unique<Sequence>();
            jumpStart->childs.emplace_back(std::make_unique<tryJumpBehaviour>());
            jumpStart->childs.emplace_back(std::make_unique<AlignBehaviour>());
            
            /* JUMP ATTACK */
            std::unique_ptr<Selector> jumpAttack = std::make_unique<Selector>();
            jumpAttack->childs.push_back(std::move(jumpUpdate));
            jumpAttack->childs.push_back(std::move(jumpStart));

            /* ATTACK */
            std::unique_ptr<Sequence> demonAttack = std::make_unique<Sequence>();
            demonAttack->childs.emplace_back(std::make_unique<DemonBehaviour>());
            demonAttack->childs.push_back(std::move(jumpAttack));
        /* DEMON BRANCH */

        /* ATTACK SELECTOR */
        std::unique_ptr<Selector> attackSelector = std::make_unique<Selector>();
        attackSelector->childs.push_back(std::move(demonAttack));
        attackSelector->childs.push_back(std::move(basicAttack));

        /* ATTACK STATE */
        std::unique_ptr<Sequence> attackState = std::make_unique<Sequence>();
        attackState->childs.emplace_back(std::make_unique<AttackStateBehaviour>());
        attackState->childs.push_back(std::move(attackSelector));
        attackState->childs.emplace_back(std::make_unique<DeletePurseBehaviour>());

    /*-- ATTACK  --*/     

    root = std::make_unique<Selector>();
    root->childs.emplace_back(std::move(lodPatrolState));
    root->childs.emplace_back(std::move(patrolState));
    root->childs.emplace_back(std::move(pursueState));
    root->childs.emplace_back(std::move(attackState));

    frame = 0;
}

void AI_System::fixedUpdate(const Context &context, float deltaTime)
{
	const vec3& player_pos = context->getPlayer().getComponent<Physics>()->position;
    ++frame;

    setPhase(context);
    setInList(context);

    duration<double> timer { 60us };

	while( timer > 0us && !schedule.empty() )
    {
        auto & ai = *(schedule.front());

        if(ai)
        {
            ai.frameCounter = frame;

            auto & enemy    = context->getEntityByID(ai.getEntityID());
            auto & physics  = *enemy.getComponent<Physics>();
            auto & velocity = *enemy.getComponent<Velocity>();

            root->run(ai, physics, velocity, player_pos, deltaTime, context);
        
            switch (ai.frequecy_state)
            {
                case 2: timer -= PATROL_PERIOD;   break;
                case 3: timer -= PURSUE_PERIOD;   break;
                case 7: timer -= ATTACK_PERIOD;   break;
            }

            ai.scheduled = false;
            schedule.pop_front();
        }
	}

    if(timer > 0us && schedule.empty())
        std::this_thread::sleep_for(timer);
}

//Scheduling 
void AI_System::setPhase(const std::unique_ptr<GameContext>& context)
{
    unsigned phase_patrol_counter = 0;
    unsigned phase_pursue_counter = 0;
    unsigned phase_attack_counter = 0;

    for (auto & ai : context->getComponents().getComponents<AI>()) 
    {
		if (ai) 
        {
            switch (ai.frequecy_state)
            {
                case 2: ai.scheduling_phase = ++phase_attack_counter;
                break;

                case 3: ai.scheduling_phase = ++phase_pursue_counter;
                break;
            
                case 7: ai.scheduling_phase = ++phase_patrol_counter;
                break;
            }
        }
    }
}

void AI_System::setInList(const std::unique_ptr<GameContext>& context)
{
    for(auto& ai : context->getComponents().getComponents<AI>())
    {
        if (ai) 
        {
            if( ai.scheduled ) 
                continue;

            if( (frame + ai.scheduling_phase) % ai.frequecy_state != 0 )
                continue;

            schedule.push_back(&ai);
            ai.scheduled = true;
        }
    }
    //sacar los enemigos que van a morir
    for(auto it=schedule.begin(); it!=schedule.end(); ++it)
    {
        auto & enemy    = context->getEntityByID((*it)->getEntityID());
        auto * cData    = enemy.getComponent<CharacterData>();

        if(!greater_e(cData->health, 0))
            it = schedule.erase(it);
    }
}

//Obstacle Avoidance
bool AI_System::checkObstacles(const vec3& ai_pos, const vec3& pj_pos, float rad, const std::unique_ptr<GameContext>& context)
{
    auto& rAABB_vector = context->getComponents().getComponents<RigidStaticAABB>();
	vec3  ecLine       = lineEcByTwoPoints(ai_pos, pj_pos - ai_pos);
    auto  maxPoint     = vec3();
    auto  minPoint     = vec3();

    maxPoint.x = (ai_pos.x > pj_pos.x) ? ai_pos.x : pj_pos.x;
    maxPoint.z = (ai_pos.z > pj_pos.z) ? ai_pos.z : pj_pos.z;

    minPoint.x = (ai_pos.x < pj_pos.x) ? ai_pos.x : pj_pos.x;
    minPoint.z = (ai_pos.z < pj_pos.z) ? ai_pos.z : pj_pos.z;

    for(auto& cmp_AABB : rAABB_vector)
    {
        if(cmp_AABB.max.x < minPoint.x) //out -x
            continue;
        if(cmp_AABB.max.z < minPoint.z) //out -z
            continue;
        if(cmp_AABB.min.x > maxPoint.x) //out +x
            continue;
        if(cmp_AABB.min.z > maxPoint.z) //out +z
            continue;

        if( lineAABBIntersectionXZ(Line(ai_pos, pj_pos), cmp_AABB.min, cmp_AABB.max) )
            return true;

        if( SphereWillIntersectBoxAABB(rad, cmp_AABB.min, cmp_AABB.max, ecLine) )
            return true;
    }

    return false;
}

bool AI_System::checkFacing(const Physics& ai_phy, const std::unique_ptr<GameContext>& context)
{
    auto& pj     = context->getPlayer();
    auto& pj_phy = *pj.getComponent<Physics>();

    auto vecRot  = getRotationYfromXZ( normalize(pj_phy.position - ai_phy.position) );

    return (vecRot - ai_phy.rotation.y < 5.f && vecRot - ai_phy.rotation.y > -5.f);
}