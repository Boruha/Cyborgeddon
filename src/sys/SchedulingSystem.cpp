#include <sys/SchedulingSystem.hpp>

void SchedulingSystem::update(const Context &context, float deltaTime)
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