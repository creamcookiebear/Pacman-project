#include <iostream>
#include <queue>
#include <unordered_set>

#include "Intersection.h" // intersection includes "Agent.h"
#include "Map.h"
#include "Agent.h"

extern Map map;

DIRECTION Ghost::navigator(Vector3f destination) const {
    // Step 1: Identify the current intersection and destination intersection
    DIRECTION currentDir;

    // Determine the current direction based on `vel`
    if (vel[0] > 0) currentDir = RIGHT;
    else if (vel[0] < 0) currentDir = LEFT;
    else if (vel[1] > 0) currentDir = UP;
    else if (vel[1] < 0) currentDir = DOWN;
    else currentDir = STAY; // Default if no velocity
    

    Intersection currentIntersection = map.getClosestIntersection(pos);
    Intersection destinationIntersection = map.getClosestIntersection(destination);

    // Use BFS or a graph traversal algorithm to find the shortest path
    std::queue<Intersection> q;
    std::unordered_map<Intersection, Intersection> parent; // To reconstruct the path
    std::unordered_set<Intersection> visited;

    q.push(currentIntersection);
    visited.insert(currentIntersection);

    while (!q.empty()) {
        Intersection intersection = q.front();
        q.pop();

        if (intersection == destinationIntersection) break; // Found the destination

        // Iterate over the neighbors of the intersection
        for (auto it = intersection.getNeighbors().begin(); it != intersection.getNeighbors().end(); ++it) {
            DIRECTION dir = it->first;
            std::array<int, 3> neighborPos = it->second;

            // Prevent 180-degree turn unless at a dead end
            if (dir == getOppositeDirection(currentDir) && intersection.getType() != Intersection::DEAD_END) {
                continue;
            }


            // Check if the neighbor has already been visited
            Intersection neighborIntersection = map.getIntersection(neighborPos);
            if (visited.find(neighborIntersection) == visited.end()) {
                visited.insert(neighborIntersection);
                parent[neighborIntersection] = intersection;
                q.push(neighborIntersection);
            }
        }
    }

    Intersection nextIntersection = destinationIntersection;
    while (parent[nextIntersection] != currentIntersection) {
        nextIntersection = parent[nextIntersection];
    }
    std::array<int, 3> interPos{ {Agent::float2map(pos)[0], Agent::float2map(pos)[1], 0} };
    std::array<int, 3> directionVector{ { nextIntersection.getPosition()[0] - interPos[0],\
        nextIntersection.getPosition()[1] - interPos[1] ,\
        nextIntersection.getPosition()[2] - interPos[2] }};

    if (directionVector[0] > 0) return RIGHT;
    if (directionVector[0] < 0) return LEFT;
    if (directionVector[1] > 0) return UP;
    if (directionVector[1] < 0) return DOWN;

    return STAY;
}

DIRECTION Ghost::getOppositeDirection(DIRECTION dir) const {
    switch (dir) {
    case UP: return DOWN;
    case DOWN: return UP;
    case LEFT: return RIGHT;
    case RIGHT: return LEFT;
    default: return STAY;
    }
}