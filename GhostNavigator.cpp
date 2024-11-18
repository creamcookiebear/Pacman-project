#include <iostream>
#include <queue>
#include <unordered_set>

#include "Intersection.h" // intersection includes "Agent.h"
#include "Map.h"
#include "Agent.h"

extern Map map;

struct BFSNode {
    Intersection intersection;
    DIRECTION cameFromDir;
};

DIRECTION Ghost::navigator(Vector3f destination) const {
    // Step 1: Identify the current intersection and destination intersection

    PRINT("Called navigator at pos : "); PRINT_POS();
    DIRECTION currentDir;

    // Determine the current direction based on `vel`
    if (vel[0] > 0) currentDir = RIGHT;
    else if (vel[0] < 0) currentDir = LEFT;
    else if (vel[1] > 0) currentDir = UP;
    else if (vel[1] < 0) currentDir = DOWN;
    else currentDir = STAY; // Default if no velocity

    Intersection currentIntersection = map.getClosestIntersection(pos);
    Intersection destinationIntersection = map.getClosestIntersection(destination);

    // Use BFS to find the shortest path
    std::queue<Intersection> q;
    std::unordered_map<Intersection, Intersection> parent;
    std::unordered_set<Intersection> visited;

    q.push(currentIntersection);
    visited.insert(currentIntersection);

    bool found = false;

    while (!q.empty() && !found) {
        Intersection intersection = q.front();
        q.pop();
        Intersection* intersection = node.intersection;

        // Check if the destination intersection is reached
        if (intersection == destinationIntersection) {
            found = true;
            break;
        }

        // Iterate over the neighbors
        for (const auto& [dir, neighborPos] : intersection.getNeighbors()) {
            /*
            if (dir == getOppositeDirection(currentDir) && intersection.getType() != Intersection::DEAD_END) {
                continue; // Skip opposite direction unless at a dead end
            }*/

            Intersection neighborIntersection = map.getIntersection(neighborPos);

            if (visited.find(neighborIntersection) == visited.end()) {
                visited.insert(neighborIntersection);
                parent[neighborIntersection] = intersection;
                q.push(neighborIntersection);
            }
        }
    }

    // If no path was found, return STAY
    if (!found) {
        PRINT("<<Critical Bug>> Path finding FAIL!"); PRINT_POS();
        return STAY;
    }
    // Backtrack to find the next intersection in the path
    Intersection nextIntersection = destinationIntersection;
    while (parent[nextIntersection] != currentIntersection) {
        nextIntersection = parent[nextIntersection];
    }

    // Determine the direction to move
    std::array<int, 3> interPos{ {Agent::float2map(pos)[0], Agent::float2map(pos)[1], 0} };
    std::array<int, 3> directionVector{
        {nextIntersection.getPosition()[0] - interPos[0],
         nextIntersection.getPosition()[1] - interPos[1],
         nextIntersection.getPosition()[2] - interPos[2]} };
    PRINT("Exit navigator at pos : "); PRINT_POS();
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