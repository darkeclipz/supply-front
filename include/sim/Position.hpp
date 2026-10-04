#pragma once

#include <cstdint>

namespace sim {

// Ground X/Z coordinates in millimeters.
struct Position {
    std::int64_t x = 0;
    std::int64_t z = 0;

    bool operator==(const Position&) const = default;
};

}