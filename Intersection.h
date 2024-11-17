#pragma once

#include "Constants.h"
#include <vector>
#include <unordered_map>
#include <array>
#include <unordered_map>
#include <functional>

class Intersection {
public:
    enum IntersectionType { FOUR_WAY, T_JUNCTION, TURN, DEAD_END, STRAIGHT };

private:
    std::array<int, 3> position;
    IntersectionType type;
    std::unordered_map<DIRECTION, std::array<int, 3>> neighbors;

public:
    Intersection()
        : position(std::array<int,3>{{0,0,0}}), type(STRAIGHT) {}
    Intersection(const std::array<int, 3>& pos, IntersectionType t)
        : position(pos), type(t) {}

    const std::array<int, 3>& getPosition() const { return position; }
    IntersectionType getType() const { return type; }

    void addNeighbor(DIRECTION dir, const std::array<int, 3>& neighborPos) {
        neighbors[dir] = neighborPos;
    }

    const std::unordered_map<DIRECTION, std::array<int, 3>>& getNeighbors() const {
        return neighbors;
    }

    bool operator==(const Intersection& inter) const {
        return inter.getPosition() == position && inter.getType() == type;
    }

    bool operator!=(const Intersection& inter) const {
        return !(inter.getPosition() == position) || !(inter.getType() == type);
    }
};

/*
std::array<int, 3> operator-(std::array<int, 3> left, std::array<int, 3> right) {
    return std::array<int, 3>{{left[0] - right[0], left[1] - right[1], left[2] - right[2] }};
}
*/

namespace std {
    template <>
    struct hash<Intersection> {
        size_t operator()(const Intersection& inter) const noexcept {
            size_t h1 = 0;
            const auto& pos = inter.getPosition();
            // Manually compute the hash for the position array
            for (const int& val : pos) {
                h1 ^= std::hash<int>{}(val)+0x9e3779b9 + (h1 << 6) + (h1 >> 2);
            }

            size_t h2 = std::hash<int>{}(static_cast<int>(inter.getType()));
            return h1 ^ (h2 << 1); // Combine the hashes
        }
    };
}