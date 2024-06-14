#include "base_edge.h"

#include <tuple>

bool BaseEdge::operator == (const BaseEdge& rhs) const {
    return std::tie(from, to, id) == std::tie(rhs.from, rhs.to, rhs.id)
           || std::tie(to, from, id) == std::tie(rhs.from, rhs.to, rhs.id);
}

std::size_t BaseEdge::BaseEdgeHasher::operator () (const BaseEdge& edge) const {
    return hasher_(edge.from) + hasher_(edge.to) + hasher_(edge.id) * 1'000'000'007; // Где 1'000'000'007 выбранная константа
}

std::size_t BaseEdgeKey::HashFromTo::operator () (const BaseEdgeKey& edge) const {
    auto lhs = hasher_(edge.from);
    auto rhs = hasher_(edge.to);

    if (lhs > rhs) {
        std::swap(lhs, rhs);
    }

    return lhs + rhs * 1'000'000'007;
}

bool BaseEdgeKey::operator == (const BaseEdgeKey& rhs) const {
    return std::tie(from, to) == std::tie(rhs.from, rhs.to)
           || std::tie(to, from) == std::tie(rhs.from, rhs.to);
}

