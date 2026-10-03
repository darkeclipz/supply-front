#pragma once

#include <cstdint>

namespace sim {

struct GameEntityId {
    std::uint64_t value = 0;
    bool operator==(const GameEntityId&) const = default;
};

}