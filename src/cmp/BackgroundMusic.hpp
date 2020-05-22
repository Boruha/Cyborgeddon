#pragma once

#include <cmp/Component.hpp>

#include <util/SoundParameter.hpp>

struct Track {
    SoundParameter sound { NO_SOUND };
    float duration { 0 };
    float timeLeft { 0 };
};

struct BackgroundMusic : public Component {
    explicit BackgroundMusic(EntityType e_type, EntityID e_ID);

    [[nodiscard]] constexpr static std::string_view getName() {
        return "BackgroundMusic";
    }

    friend std::ostream& operator<<(std::ostream& os, const BackgroundMusic& music);

    std::array<Track, 4> tracks {};
};
