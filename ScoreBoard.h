#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <GL/freeglut.h>

class ScoreManager {
private:
    struct Player {
        std::string name;
        int score;

        // Constructor for convenience
        Player() {};
        Player(const std::string& name, int score) : name(name), score(score) {}
    };

    std::string filename;
    std::vector<Player> players;

    static bool comparePlayers(const Player& a, const Player& b) {
        return a.score > b.score;
    }

public:
    ScoreManager(const std::string& file) : filename(file) {
        loadScores();
    }

    void displayText(float x, float y, std::string str) {
        glRasterPos2f(x, y);
        for (char c : str) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    void loadScores() {
        players.clear();
        std::ifstream file(filename);

        if (file.is_open()) {
            std::string name;
            int score;
            while (file >> name >> score) {
                players.emplace_back(name, score);
            }
            file.close();
        }
        else {
            std::cerr << "Could not open file: " << filename << std::endl;
        }
    }

    void saveScores() const {
        std::ofstream file(filename);

        if (file.is_open()) {
            for (const auto& player : players) {
                file << player.name << " " << player.score << std::endl;
            }
            file.close();
        }
        else {
            std::cerr << "Could not write to file: " << filename << std::endl;
        }
    }

    
    void addScore(const std::string& name, int score) {
        players.emplace_back(name, score);
        std::sort(players.begin(), players.end(), comparePlayers);
        if (players.size() > 10) {
            players.resize(10);
        }
    }
    void displayScores() {
        glColor3f(1.0, 1.0, 1.0); // White color

        std::string top = "Top Scores";

        displayText(-100.f, 200, top);
        float y = 160.f;
        for (size_t i = 0; i < players.size(); ++i) {
            std::string line; std::string inter = " : ";
            line = players[i].name + inter + std::to_string(players[i].score);
            displayText(-100.f, y, line);
            y -= 20.f;
        }
        
    }
};