#pragma once

#include "sim/GameEntityId.hpp"

#include <vector>

namespace app {

class Selection {
public:
    void select(sim::GameEntityId id);
    void add(sim::GameEntityId id);
    void remove(sim::GameEntityId id);
    void clear() noexcept;
    [[nodiscard]] bool contains(sim::GameEntityId id) const;
    [[nodiscard]] const std::vector<sim::GameEntityId>& entities() const noexcept;
private:
    std::vector<sim::GameEntityId> m_entities;
};

}