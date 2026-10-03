#pragma once

#include "sim/GameEntityId.hpp"
#include "sim/Tick.hpp"

#include <cstdint>

namespace sim{

struct DestroyEntityCommand {
    GameEntityId target;
};

struct Command {
    Tick execute_at = 0;
    std::uint32_t player_id = 0;
    std::uint64_t sequence = 0;
    DestroyEntityCommand payload;
};

enum class CommandSubmission {
    queued,
    invalid_tick,
    invalid_sequence
};

}