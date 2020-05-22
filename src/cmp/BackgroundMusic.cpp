#include <cmp/BackgroundMusic.hpp>

#include <ostream>

BackgroundMusic::BackgroundMusic(const EntityType type, const EntityID e_ID)
        : Component(type, e_ID) { }


std::ostream& operator<<(std::ostream& os, const BackgroundMusic& music)
{
    music.print(os, BackgroundMusic::getName());

    for(unsigned i = 0; i < music.tracks.size(); ++i )
        os  << "\n\tSound: " << i << music.tracks[i].sound
            << "\n\tDuration: "  << music.tracks[i].duration
            << "\n\tTimeLeft "   << music.tracks[i].timeLeft;


    return os;
}