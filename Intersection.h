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
        return inter.getPosition() == position;
    }

    bool operator!=(const Intersection& inter) const {
        return !(*this==inter);
    }
};


namespace std {
    template <>
    struct hash<Intersection> {
        size_t operator()(const Intersection& inter) const noexcept {
            size_t h1 = 0;
            const auto& pos = inter.getPosition();
            // Compute the hash for the position array only
            for (const int& val : pos) {
                h1 ^= std::hash<int>{}(val)+0x9e3779b9 + (h1 << 6) + (h1 >> 2);
            }
            return h1;
        }
    };
}

namespace std {
    template <>
    struct hash<std::array<int, 3>> {
        size_t operator()(const std::array<int, 3>& arr) const noexcept {
            size_t h = 0;
            for (const int& val : arr) {
                // Combine the hash of each element into the total hash
                h ^= std::hash<int>{}(val)+0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}