#include <iostream>
#include <queue>
#include <unordered_set>
#include <map>

#include "Intersection.h" // intersection includes "Agent.h"
#include "Map.h"
#include "Agent.h"

extern Map map;

struct BFSNode {
    std::array<int, 3> position;
    DIRECTION cameFromDir;
};

DIRECTION Ghost::navigator(Vector3f destination) const {
    PRINT("Called navigator at pos : "); PRINT_POS();
    DIRECTION currentDir;

    // Determine current direction based on vel
    if (pastVel[0] > 0) currentDir = RIGHT;
    else if (pastVel[0] < 0) currentDir = LEFT;
    else if (pastVel[1] > 0) currentDir = UP;
    else if (pastVel[1] < 0) currentDir = DOWN;
    else currentDir = STAY; // Default if no velocity

    if (Agent::float2map(destination) == Agent::float2map(pos)) {
        // if at destination
        PRINT("====================\nAt Destination\n=====================" << std::endl);
        std::map<DIRECTION, bool> ableDir;
        std::array<int, 2> tempIdx = { {Agent::float2map(pos)[0],Agent::float2map(pos)[1]} };
        ableDir[UP] = (Map::isInbound(tempIdx[0], tempIdx[1] + 1) && !map.W(tempIdx[0], tempIdx[1] + 1));
        ableDir[DOWN] = (Map::isInbound(tempIdx[0], tempIdx[1] - 1) && !map.W(tempIdx[0], tempIdx[1] - 1));
        ableDir[RIGHT] = (Map::isInbound(tempIdx[0] + 1, tempIdx[1]) && !map.W(tempIdx[0] + 1, tempIdx[1]));
        ableDir[DOWN] = (Map::isInbound(tempIdx[0] - 1, tempIdx[1]) && !map.W(tempIdx[0] - 1, tempIdx[1]));
        
        for (DIRECTION dir : std::array<DIRECTION,4>{{UP,DOWN,RIGHT,LEFT}}) {
            PRINT(ableDir[dir] << std::endl;);
            PRINT("dir : " << dir << std::endl);
            if ((dir != currentDir || ableDir[dir]) && dir \
                != Ghost::getOppositeDirection(currentDir) && ableDir[dir]) {
                PRINT("Destination ducktape case, dir : " << dir << std::endl);
                return dir;
            }
        }
        PRINT("Error at escaping" << std::endl);
        return (DIRECTION)((currentDir + 1) % 4);
    }

    Intersection currentIntersection = map.getClosestIntersection(pos);
    Intersection destinationIntersection = map.getClosestIntersection(destination);
    PRINT("Destination Pos: ");
    PRINT(destinationIntersection.getPosition()[0] << ", " << destinationIntersection.getPosition()[1] << std::endl);

    std::queue<BFSNode> q;
    q.push({ currentIntersection.getPosition(), getOppositeDirection(currentDir) });


    std::unordered_set<std::array<int, 3>> visited;
    std::unordered_map<std::array<int, 3>, std::array<int, 3>> parent;
    
    visited.insert(currentIntersection.getPosition());

    bool found = false;

    while (!q.empty() && !found) {
        BFSNode node = q.front();
        q.pop();

        if (node.position == destinationIntersection.getPosition()) {
            found = true;
            break;
        }

        Intersection& intersection = map.getIntersection(node.position);

        for (const auto& [dir, neighborPos] : intersection.getNeighbors()) {
            if (dir == node.cameFromDir) {
                continue; // Avoid immediate U-turn
            }

            if (visited.find(neighborPos) == visited.end()) {
                visited.insert(neighborPos);
                parent[neighborPos] = node.position;
                q.push({ neighborPos, getOppositeDirection(dir) });
            }
        }
    }

    if (!found) {
        PRINT("<<Critical Bug>> Path finding FAIL!"); PRINT_POS();
        return STAY;
    }

    // Reconstruct the path
    std::array<int, 3> nextPos = destinationIntersection.getPosition();
    while (parent[nextPos] != currentIntersection.getPosition()) {
        nextPos = parent[nextPos];
    }

    std::array<int, 3> interPos = currentIntersection.getPosition();
    std::array<int, 3> directionVector{
        {nextPos[0] - interPos[0],
         nextPos[1] - interPos[1],
         0}
    };

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