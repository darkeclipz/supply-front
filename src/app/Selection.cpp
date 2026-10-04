#include "app/Selection.hpp"

#include <algorithm>

namespace app {

void Selection::select(sim::GameEntityId id) {
    clear();
    add(id);
}

void Selection::add(sim::GameEntityId id) {
    if (id.value != 0 && !contains(id)) {
        m_entities.push_back(id);
    }
}

void Selection::remove(sim::GameEntityId id) {
    std::erase(m_entities, id);
}

void Selection::clear() noexcept {
    m_entities.clear();
}

bool Selection::contains(sim::GameEntityId id) const {
    return std::find(m_entities.begin(), m_entities.end(), id) != m_entities.end();
}

const std::vector<sim::GameEntityId>& Selection::entities() const noexcept {
    return m_entities;
}

}