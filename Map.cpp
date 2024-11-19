#include "Map.h"
#include <iostream>

Map::Map() {
    for (int w = 0; w < hight; w++) {
        arrMap.push_back(std::vector<std::array<bool, 4>>());
        for (int h = 0; h < width; h++) {
            arrMap[w].push_back(std::array<bool, 4> {{false, false, false, false}});
        }
    }
}

Vector3f Map::map2float(int x, int y) {
    float xf = -Map::width * Map::blockSize / 2.f + Map::blockSize / 2.f + x * Map::blockSize;
    float yf = -Map::hight * Map::blockSize / 2.f + Map::blockSize / 2.f + y * Map::blockSize;
    return Vector3f(xf, yf, 0.f);
}

std::array<int, 2> Map::float2map(Vector3f p) { //get nearest block idx
    float xf = p[0]; float yf = p[1];
    int x, y;
    x = static_cast<int>(0.5 * (2.f * xf / Map::blockSize + Map::width - 1) + 0.5);
    y = static_cast<int>(0.5 * (2.f * yf / Map::blockSize + Map::hight - 1) + 0.5);
    return std::array<int, 2> {{x, y}};
}

bool Map::isInbound(int x, int y) {
    return (x >= 0) && (x <= Map::width - 1) && (y >= 0) && (y <= Map::hight - 1);
}
bool Map::isInbound(std::array<int, 2> idxPos) {
    return Map::isInbound(idxPos[0], idxPos[1]);
}


void Map::sW(int i, int j, bool val) { //set Wall, single element
	arrMap[Map::toMap(i, j)[0]][Map::toMap(i, j)[1]][0] = val;
}
void Map::sW(Vector3i pos, bool val) {
	arrMap[pos[1]][pos[0]][0] = val;
}

bool Map::W(int x, int y) const {
    if (!Map::isInbound(x, y)) return true; // if out of Map bound. return true.
    return arrMap[Map::toMap(x, y)[0]][Map::toMap(x, y)[1]][0];
}

bool Map::W(Vector3i pos) const { //get Pellet by element, by Vec3i
    if (!Map::isInbound(pos[0], pos[1])) return true; // if out of Map bound. return true.
	return arrMap[Map::toMap(pos[0], pos[1])[0]][Map::toMap(pos[0], pos[1])[1]][0];
}

bool Map::W(Vector3f pos) const { //get Pellet by element, by Vec3i
    int x, y;
    x = Map::toMap(Map::float2map(pos)[0], Map::float2map(pos)[1])[0];
    y = Map::toMap(Map::float2map(pos)[0], Map::float2map(pos)[1])[1];
    return arrMap[x][y][0];
}

Intersection Map::getClosestIntersection(const Vector3f& pos) const {
    int xi = Map::float2map(pos)[0]; int yi = Map::float2map(pos)[1];
    std::array<int, 3> interPos = { {xi, yi, 0} };
    for (auto it = intersections.begin(); it != intersections.end(); it++) {
        if (it->getPosition() == interPos) return *it;
    }
    std::cout << "No matching intersection.\nError case." << std::endl;
    return Intersection(std::array<int, 3>{{0, 0, 0}}, Intersection::STRAIGHT); // error case
}

Intersection& Map::getIntersection(const std::array<int, 3>& interPos) {
    for (auto& intersection : intersections) {
        if (intersection.getPosition() == interPos) {
            return intersection;
        }
    }
    std::cout << "No matching intersection.\nError case." << std::endl;
    throw std::runtime_error("Intersection not found");
}

void Map::sP(int i, int j, bool val) { //set Pellet, single element
    arrMap[Map::toMap(i, j)[0]][Map::toMap(i, j)[1]][1] = val;
}
void Map::sP(Vector3i pos, bool val) {
	arrMap[pos[0]][pos[1]][1] = val;
}
bool Map::P(int x, int y) const {
    return arrMap[Map::toMap(x, y)[0]][Map::toMap(x, y)[1]][1];
}
bool Map::P(Vector3i pos) const { //get Pellet by element, by Vec3i
	return arrMap[Map::toMap(pos[0], pos[1])[0]][Map::toMap(pos[0], pos[1])[1]][1];
}

void Map::sPow(int i, int j, bool val) { //set Pellet, single element
    arrMap[Map::toMap(i, j)[0]][Map::toMap(i, j)[1]][2] = val;
}
void Map::sPow(Vector3i pos, bool val) {
	arrMap[pos[0]][pos[1]][2] = val;
}
bool Map::Pow(int x, int y) const {
    return arrMap[Map::toMap(x, y)[0]][Map::toMap(x, y)[1]][2];
}
bool Map::Pow(Vector3i pos) const { //get Pellet by element, by Vec3i
    return arrMap[Map::toMap(pos[0], pos[1])[0]][Map::toMap(pos[0], pos[1])[1]][2];
}

void Map::generateIntersections() {
    for (int y = 0; y < Map::hight; ++y) { // y ranges from 0 to hight - 1
        for (int x = 0; x < Map::width; ++x) { // x ranges from 0 to width - 1

            if (arrMap[Map::hight - 1 - y][x][0]) continue; // Skip walls

            // Check neighboring cells
            int openPaths = 0;
            bool up = (y < Map::hight - 1 && !W(x, y + 1));
            bool down = (y > 0 && !W(x, y - 1));
            bool left = (x > 0 && !W(x - 1, y));
            bool right = (x < Map::width - 1 && !W(x + 1, y));

            openPaths += up + down + left + right;

            Intersection::IntersectionType type;
            if (openPaths == 4) {
                type = Intersection::FOUR_WAY;
            }
            else if (openPaths == 3) {
                type = Intersection::T_JUNCTION;
            }
            else if (openPaths == 2) {
                if ((up && down) || (left && right)) {
                    type = Intersection::STRAIGHT;
                }
                else {
                    type = Intersection::TURN;
                }
            }
            else if (openPaths == 1) {
                type = Intersection::DEAD_END;
            }
            else {
                continue; // Not an intersection
            }
            
            std::array<int, 3> pos{ {x, y, 0} };
            Intersection intersection(pos, type);

            if (up) intersection.addNeighbor(UP, std::array<int, 3>{{x, y + 1, 0}});
            if (down) intersection.addNeighbor(DOWN, std::array<int, 3>{{x, y - 1, 0}});
            if (left) intersection.addNeighbor(LEFT, std::array<int, 3>{{x - 1, y, 0}});
            if (right) intersection.addNeighbor(RIGHT, std::array<int, 3>{{x + 1, y, 0}});

            intersections.push_back(intersection);
        }
    }

    // After generating all intersections, Validate it.
    std::cout << "Generated Intersections and their Neighbors:\n";
    for (const auto& intersection : intersections) {
        const auto& pos = intersection.getPosition();
        std::cout << "Intersection at (" << pos[0] << ", " << pos[1] << "):\n";

        for (const auto& [dir, neighborPos] : intersection.getNeighbors()) {
            std::string dirStr;
            switch (dir) {
            case UP: dirStr = "UP"; break;
            case DOWN: dirStr = "DOWN"; break;
            case LEFT: dirStr = "LEFT"; break;
            case RIGHT: dirStr = "RIGHT"; break;
            default: dirStr = "UNKNOWN";
            }
            std::cout << "  Neighbor " << dirStr << " at (" << neighborPos[0] << ", " << neighborPos[1] << ")\n";
        }
    }
}

void Map::setMtl_wall(Material mtl) {
    mtl_wall = mtl;
}
void Map::setMtl_pellet(Material mtl) {
    mtl_pellet = mtl;
}
void Map::setMtl_power(Material mtl) {
    mtl_power = mtl;
}

void Map::MapInitializer(const std::string& filename) {
    // load map for once
    loadMapFromFile(filename, Map::hight, Map::width);
    //
    generateIntersections();
}

void Map::loadMapFromFile(const std::string& filename, int width, int height) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    arrMap.resize(width, std::vector<std::array<bool, 4>>(height));

    std::string line;
    for (int w = 0; w < width && std::getline(file, line); w++) {
        std::istringstream stream(line);
        for (int h = 0; h < height; h++) {
            int encodedValue;
            if (!(stream >> encodedValue) || encodedValue < 0 || encodedValue > 3) {
                std::cerr << "Invalid encoded value at (" << w << ", " << h << ")" << std::endl;
                PRINT(encodedValue << std::endl);
                return;
            }

            std::array<bool, 4> tileState = { false, false, false, false };
            tileState[encodedValue] = true;

            arrMap[w][h] = tileState;
            
            if (stream.peek() == ',') {
                stream.ignore();
            }
        }
    }

    file.close();

    std::cout << "Loaded Map Config: \n" << std::endl;
    for (const auto& row : arrMap) {
        for (const auto& col : row) {
            char type;
            if (col[0]) type = 'B';
            else if (col[1]) type = '.';
            else if (col[2]) type = 'o';
            else type = ' ';
            std::cout << type;
        }
        std::cout << std::endl;
    }
}

void Map::draw() {
    int x, y;
    for (int ym = 0; ym < Map::hight; ym++) {
        for (int xm = 0; xm < Map::width; xm++) {
            y = Map::hight-1 - ym; x = xm;
            if (arrMap[ym][xm][0]) drawWall(Map::map2float(x, y)); // draw wall
            else if (arrMap[ym][xm][1]) drawPellet(Map::map2float(x, y)); // draw pellet
            else if (arrMap[ym][xm][2]) drawPower(Map::map2float(x, y)); // draw Power
        }
    }
}

void Map::drawWall(Vector3f pos) {
    //std::cout << "draw wall: ";
    //pos.print();
    glMaterialfv(GL_FRONT, GL_EMISSION, mtl_wall.getEmission().getPos());
    glMaterialfv(GL_FRONT, GL_AMBIENT, mtl_wall.getAmbient().getPos());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl_wall.getDiffuse().getPos());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mtl_wall.getSpecular().getPos());
    glMaterialfv(GL_FRONT, GL_SHININESS, mtl_wall.getShininess());

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2] + Map::blockSize / 2.f);
    glutSolidCube(Map::blockSize);
    glPopMatrix();
}

void Map::drawPellet(Vector3f pos) {
    //std::cout << "draw pel: ";
    //pos.print();
    glMaterialfv(GL_FRONT, GL_EMISSION, mtl_pellet.getEmission().getPos());
    glMaterialfv(GL_FRONT, GL_AMBIENT, mtl_pellet.getAmbient().getPos());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl_pellet.getDiffuse().getPos());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mtl_pellet.getSpecular().getPos());
    glMaterialfv(GL_FRONT, GL_SHININESS, mtl_pellet.getShininess());

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2] + Map::blockSize / 2.f);
    glutSolidCube(Map::blockSize/5);
    glPopMatrix();
}

void Map::drawPower(Vector3f pos) {
    //std::cout << "draw pow: ";
    //pos.print();
    glMaterialfv(GL_FRONT, GL_EMISSION, mtl_power.getEmission().getPos());
    glMaterialfv(GL_FRONT, GL_AMBIENT, mtl_power.getAmbient().getPos());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl_power.getDiffuse().getPos());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mtl_power.getSpecular().getPos());
    glMaterialfv(GL_FRONT, GL_SHININESS, mtl_power.getShininess());
    
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2] + Map::blockSize / 2.f);
    glutSolidCube(Map::blockSize / 3);
    glPopMatrix();
}