#pragma once

#include <Engine/util/Enum.hpp>
#include <cstdint>

struct Color {
    explicit Color(const COLOR color) : r(color >> 24u & 0xFFu), g(color >> 16u & 0xFFu), b(color >> 8u & 0xFFu), a(color & 0xFFu) {  }

    uint8_t r { 0x00 }, g { 0x00 }, b { 0x00 }, a { 0xFF };
};